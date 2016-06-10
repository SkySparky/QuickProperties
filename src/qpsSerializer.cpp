/*
    This file is part of QuickProperties2 library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickProperties2 library.
//
// \file	qpsSerializer.cpp
// \author	benoit@qanava.org
// \date	2016 01 20
//-----------------------------------------------------------------------------

// QuickProperties headers
#include "./qpsSerializer.h"

// STD headers
#include <iostream>

// Qt headers
#include <QColor>

namespace qps { // ::qps

auto ProtoSerializer::serializeOut( const qps::Properties& properties,
                                    qps::pb::Properties& pbProperties ) -> void
{
    pbProperties.set_object_name( properties.objectName().toStdString() );
    pbProperties.set_hidden_static_properties_count( properties.getHiddenStaticPropertiesCount() );

    // Serialize static properties
    int pCount = properties.metaObject( )->propertyCount( );
    pbProperties.set_static_property_count( pCount );
    for ( int i = properties.getHiddenStaticPropertiesCount(); i < pCount; ++i ) {
        auto pbProperty = pbProperties.add_properties();
        if ( pbProperty == nullptr )
            continue;
        QMetaProperty metaProperty = properties.metaObject( )->property( i );
        QVariant v = properties.property( metaProperty.name( ) );
        QString vs = variantToString( v );
        pbProperty->set_name( std::string( metaProperty.name() ) );
        pbProperty->set_type( v.type() );
        pbProperty->set_value( vs.toStdString() );
    }

    // Serialize dynamic properties
    QList< QByteArray > dynamicProperties = properties.dynamicPropertyNames( );
    pbProperties.set_static_property_count( dynamicProperties.size() );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        auto pbProperty = pbProperties.add_properties();
        if ( pbProperty == nullptr )
            return;
        QString propertyName = dynamicProperties.at( d );
        QVariant v = properties.property( propertyName.toLatin1( ) );
        QString vs = variantToString( v );
        pbProperty->set_name( propertyName.toStdString() );
        pbProperty->set_type( v.type() );
        pbProperty->set_value( vs.toStdString() );
    }
}

auto ProtoSerializer::serializeIn( const qps::pb::Properties& pbProperties,
                                   qps::Properties& properties ) -> void
{
    for ( auto& pbProperty : pbProperties.properties() ) {
        QVariant value = variantFromString( pbProperty.type(),
                                            QString::fromStdString( pbProperty.value() ) );
        if ( value.isValid() )
            properties.setProperty( QString::fromStdString( pbProperty.name() ),
                                    value );
    }
}

auto    ProtoSerializer::variantToString( QVariant v ) -> QString
{
    QString vs;
    switch ( v.type( ) ) {
    case QVariant::Type::SizeF: {
        QSizeF s = v.toSizeF( );
        QTextStream vss( &vs );
        vss << s.width( ) << " x " << s.height( );
        break;
    }
    default:
        vs = v.toString( );
    }
    return vs;
}

auto    ProtoSerializer::variantFromString( int type, QString s ) -> QVariant
{
    if ( s.isEmpty() )
        return QVariant{};
    QVariant value{};
    switch ( type ) {
    case QVariant::Double:
        value = s.toDouble();
        break;
    case QVariant::Bool:
        value = ( s == "true" );
        break;
    case QVariant::Int:
        value = s.toInt();
        break;
    case QVariant::String:
        value = s;
        break;
    case QVariant::Color: {
        QColor c( s );
        if ( c.isValid( ) )
            value = QVariant::fromValue< QColor >( c );
    }
        break;
    case QVariant::SizeF: {
            QTextStream ts( &s );
            qreal w; ts >> w;
            QString separator; ts >> separator;
            qreal h; ts >> h;
            value = QVariant::fromValue< QSizeF >( QSizeF( w, h ) );
        }
        break;
    default:
        std::cerr << "qps::ProtoSerializer::variantFromString(): Warning: Unknown variant string " << s.toStdString() << " with type=" << type << std::endl;
        break;
    }
    return value;
}

auto ProtoSerializer::serializeOutTimed( const qps::TimedProperties& properties,
                                         qps::pb::TimedProperties& pbProperties ) -> void
{
    if ( pbProperties.mutable_base() != nullptr )
        serializeOut( properties, *pbProperties.mutable_base() );    // Serialize simple properties

    pbProperties.set_first( properties.getFirst().isValid() ? properties.getFirst().toMSecsSinceEpoch() : -1 );
    pbProperties.set_last( properties.getLast().isValid() ? properties.getLast().toMSecsSinceEpoch() : -1 );

    int pCount = properties.metaObject( )->propertyCount( );
    for ( int i = properties.getHiddenStaticPropertiesCount(); i < pCount; ++i ) {
        QMetaProperty metaProperty = properties.metaObject( )->property( i );
        QString propertyName( metaProperty.name( ) );
        if ( propertyName.isEmpty() )
            continue;
        const qps::TimeValueMap* tvm = properties.getConstTimeValueMap( propertyName );
        if ( tvm == nullptr )
            continue;
        qDebug() << "Serializing out TVM for static property: " << propertyName;
        qps::pb::TimeValueMap* pbTvm = pbProperties.add_tvms();
        if ( pbTvm != nullptr )
            serializeOutTvm( *tvm, propertyName, *pbTvm );
    }

    // Serialize dynamic properties
    QList< QByteArray > dynamicProperties = properties.dynamicPropertyNames( );
    for ( int d = 0; d < dynamicProperties.size( ); d++ ) {
        QString propertyName( dynamicProperties.at( d ) );
        if ( propertyName.isEmpty() )
            continue;
        const qps::TimeValueMap* tvm = properties.getConstTimeValueMap( propertyName );
        if ( tvm == nullptr )
            continue;
        qps::pb::TimeValueMap* pbTvm = pbProperties.add_tvms();
        if ( pbTvm != nullptr )
            serializeOutTvm( *tvm, propertyName, *pbTvm );
    }
}

auto ProtoSerializer::serializeInTimed( const qps::pb::TimedProperties& pbProperties,
                                        qps::TimedProperties& properties ) -> void
{
    serializeIn( pbProperties.base(), static_cast<qps::Properties&>( properties ) );
    QDateTime first = ( pbProperties.first() != -1 ? QDateTime::fromMSecsSinceEpoch(pbProperties.first()) :
                                                     QDateTime() );
    properties.setFirst(first);

    QDateTime last = ( pbProperties.last() != -1 ? QDateTime::fromMSecsSinceEpoch(pbProperties.last()) :
                                                   QDateTime() );
    properties.setLast(last);

    for ( auto& pbTvmIter : pbProperties.tvms() ) {
        QString tvmPropertyName = QString::fromStdString( pbTvmIter.property_name() );
        qps::TimeValueMap* tvm = properties.getTimeValueMap( tvmPropertyName );
        if ( tvm != nullptr )
            serializeInTvm( pbTvmIter, *tvm );
    }
}

auto ProtoSerializer::serializeOutTvm( const qps::TimeValueMap& tvm, QString propertyName, qps::pb::TimeValueMap& pbTvm ) -> void
{
    pbTvm.set_property_name( propertyName.toStdString() );
    if ( tvm.size() <= 0 )
        return;

    // Get TVM value type
    QVariant::Type tvmType = tvm.first().type();
    pbTvm.set_property_type( tvmType );

    // Serialize all tvm, serialize double TVM as concrete protobuf double map, convert to string for all other values type
    if ( tvmType == QVariant::Double ) {
        std::map< qint64, double > doubleTvm;
        qps::pb::TimeDoubleMap pbDoubleTvm;
        qps::TimeValueMap::const_iterator i = tvm.constBegin();
        while ( i != tvm.constEnd() ) {
            doubleTvm.insert( std::make_pair( i.key().toMSecsSinceEpoch(), i.value().toDouble( ) ) );
            ++i;
        }
        pbDoubleTvm.mutable_values()->insert( doubleTvm.begin(), doubleTvm.end() );
        pbTvm.mutable_map()->PackFrom( pbDoubleTvm );
    } else {
        std::map< qint64, std::string > stringTvm;
        qps::pb::TimeStringMap pbStringTvm;
        qps::TimeValueMap::const_iterator i = tvm.constBegin();
        while ( i != tvm.constEnd() ) {
            stringTvm.insert( std::make_pair( i.key().toMSecsSinceEpoch(),
                                              variantToString( i.value() ).toStdString() ) );
            ++i;
        }
        pbStringTvm.mutable_values()->insert( stringTvm.begin(), stringTvm.end() );
        pbTvm.mutable_map()->PackFrom( pbStringTvm );
    }
}

auto ProtoSerializer::serializeInTvm( const qps::pb::TimeValueMap& pbTvm, qps::TimeValueMap& tvm ) -> void
{
    if ( pbTvm.property_type() == QVariant::Double ) {
        if ( pbTvm.map().Is< qps::pb::TimeDoubleMap >() ) {
            qps::pb::TimeDoubleMap pbDoubleTvm;
            if ( pbTvm.map().UnpackTo( &pbDoubleTvm ) ) {
                for ( auto doubleTvmIter : pbDoubleTvm.values() ) {
                    qint64 t = doubleTvmIter.first;
                    double d = doubleTvmIter.second;
                    tvm.insert( QDateTime::fromMSecsSinceEpoch( t ), QVariant::fromValue< double >( d ) );
                }
            }
        }
    } else {
        if ( pbTvm.map().Is< qps::pb::TimeStringMap >() ) {
            qps::pb::TimeStringMap pbStringTvm;
            if ( pbTvm.map().UnpackTo( &pbStringTvm ) ) {
                for ( auto stringTvmIter : pbStringTvm.values() ) {
                    qint64      t = stringTvmIter.first;
                    std::string s = stringTvmIter.second;
                    tvm.insert( QDateTime::fromMSecsSinceEpoch( t ),
                                variantFromString( pbTvm.property_type(), QString::fromStdString( s ) ) );
                }
            }
        }
    }
}

} // ::qps



