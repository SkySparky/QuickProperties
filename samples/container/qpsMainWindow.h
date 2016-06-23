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
// \file	qpsMainWindow.h
// \author	benoit@qanava.org
// \date	2015 10 29
//-----------------------------------------------------------------------------

#ifndef qpsMainWindow_h
#define qpsMainWindow_h

// Qt headers
#include <QObject>
#include <QGuiApplication>
#include <QQuickView>

// QuickProperties headers
#include "../../src/QuickProperties2.h"

class Dummy : public QObject
{
    Q_OBJECT
public:
    Dummy( ) : QObject( nullptr ) { }
    explicit Dummy( QString label, double number ) :
        QObject( nullptr ), _label( label ), _number{ number } { }
    virtual ~Dummy( ) { }
private:
    Q_DISABLE_COPY( Dummy )

    // User defined "static" properties
public:
    Q_PROPERTY( QString  label READ getLabel WRITE setLabel NOTIFY labelChanged )
    QString     getLabel( ) const { return _label; }
    void        setLabel( QString label ) { _label = label; emit labelChanged( ); }
signals:
    void        labelChanged( );
protected:
    QString      _label{""};

public:
    Q_PROPERTY( double  number READ getNumber WRITE setNumber NOTIFY numberChanged )
    double      getNumber( ) const { return _number; }
    void        setNumber( double number ) { _number = number; emit numberChanged( ); }
signals:
    void        numberChanged( );
protected:
    double      _number{42.0};
};

class MainView : public QQuickView
{
    Q_OBJECT
public:
    MainView( QGuiApplication* application );
    virtual ~MainView( ) { }
private:
    Q_DISABLE_COPY( MainView )
};

#endif // qpsMainWindow_h

