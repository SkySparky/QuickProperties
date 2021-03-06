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
// \file	qpsPropertiesList.cpp
// \author	benoit@qanava.org
// \date	2015 06 20
//-----------------------------------------------------------------------------

// Qt headers
#include <QFont>

// QuickProperties headers
#include "./qpsPropertiesList.h"

namespace qps { // ::qps

/* PropertiesList Object Management *///---------------------------------------
PropertiesList::PropertiesList( QObject* parent )
    : qps::ContainerListModel< QVector, qps::Properties* >( parent )/*,
    _displayRolePropertyIndex( -1 )*/
{

}
//-----------------------------------------------------------------------------

} // ::qps

