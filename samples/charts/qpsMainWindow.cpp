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
// \file	qpsMainWindow.cpp
// \author	benoit@qanava.org
// \date	2015 10 18
//-----------------------------------------------------------------------------

// Qt headers
#include <QApplication>
#include <QQmlContext>

#include <QQuickItem>
#include <QtCharts/QChart>

// QuickProperties headers
#include "./qpsMainWindow.h"
#include "../src/qpsTimeValueMap.h"

using namespace qps;

template < typename T = double >
void    generateRandTimeSinValues( qps::TimeValueMap& tvm, int numValues, qint64 interval )
{
    QDateTime current = QDateTime::currentDateTimeUtc( );
    QDateTime time = current.addMSecs( -interval );
    for ( int v = 0; v < numValues; ++v )
    {
        T value = sin( v / (qreal)numValues * 3.1415926 * 2. );
        tvm.qpsInsert( time, value );
        time = time.addMSecs( qRound( ( double )interval / ( double )numValues ) );
    }
}

//-----------------------------------------------------------------------------
MainView::MainView( QGuiApplication* ) :
    QQuickView( )
{
    QScopedPointer< qps::AbstractTranslator > translator{ new qps::AbstractTranslator() };
    QuickProperties::initialize( engine(), translator.data() );

    // Create a test data set
    qps::TimeValueMap* tvm = new qps::TimeValueMap( this );
    //generateRandTimeSinValues( *tvm, 155, 50000 );

    //rootContext( )->setContextProperty( "ls", ls );

    setSource( QUrl( "qrc:/charts.qml" ) );
    qDebug() << "findChild(test)=" << rootObject()->findChild<QObject*>( "test" );
    qDebug() << "findChild(chartview)=" << rootObject()->findChild<QObject*>( "chartview" );

    QLineSeries* ls = nullptr;
    QObject* cv = rootObject()->findChild<QObject*>( "chartview" );
    if ( cv != nullptr ) {
        QVariant lsv = cv->property( "lineSeries" );
        qDebug() << "lsv=" << lsv;
        ls = qobject_cast<QLineSeries*>( lsv.value<QObject*>() );
        qDebug() << "ls=" << ls;
    }

//    qDebug() << "findChild(lineseries)=" << rootObject()->findChild<QObject*>( "lineseries" );
//    qDebug() << "findChild(lineseries)=" << rootObject()->findChild<QLineSeries*>( "lineseries" );
//    QLineSeries* ls = qobject_cast< QLineSeries* >( rootObject( )->findChild< QLineSeries* >( "ls" ) );
//    qDebug() << "ls=" << ls;

    if ( ls != nullptr ) {
        tvm->setLineSeries( ls );

        qint64 t = QDateTime::currentDateTime().toMSecsSinceEpoch();
        qDebug() << "t=" << t;
        ls->append( 0, 0.5 );
        ls->append( 1, 0.6 );
        ls->append( 2, 0.7 );
        ls->append( 3, 0.8 );
        ls->append( 4, 0.9 );

        ls->append( t, 0.5 );
        ls->append( t+1000, 0.5 );
        ls->append( t+20000, 0.5 );
        ls->append( t+30000, 0.6 );
        ls->append( t+40000, 0.5 );
        ls->append( t+60000, 0.8 );
    }
    generateRandTimeSinValues( *tvm, 155, 50000 );

}
//-----------------------------------------------------------------------------

int	main( int argc, char** argv )
{
    QApplication app(argc, argv);

    MainView mainView( &app );
    mainView.setResizeMode( QQuickView::SizeRootObjectToView );
    mainView.resize( 1084, 530 );
    mainView.show( );

    return app.exec( );
}
