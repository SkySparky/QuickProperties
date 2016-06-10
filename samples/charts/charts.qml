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

import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QuickProperties 2.0 as Qps
import "qrc:/QuickProperties2" as Qps
import QtCharts 2.0

Item {
    id: mainVindow
    anchors.fill: parent
    // DateTimeAxis is based on QDateTimes so we must convert our JavaScript dates to
    // milliseconds since epoch to make them match the DateTimeAxis values
    function toMsecsSinceEpoch(date) {
        var msecs = date.getTime();
        return msecs;
    }

    QtObject {
        objectName: "test"
    }

    ChartView {
        title: "TVM Interface Test"
        anchors.fill: parent
        antialiasing: true
        id: cv
        objectName: "chartview"

        property var lineSeries: cv.series(0)

        Component.onCompleted: lineSeries = cv.series(0)
        LineSeries {
            id: ls
            objectName: "lineseries"
            axisX: DateTimeAxis {
                format: "yyyy MMM"
                tickCount: 5
                //onMinChanged: console.debug("min=" + min)
                //onMaxChanged: console.debug("max=" + max)
                min: new Date()
                max: new Date(2016, 3, 23)
            }
            axisY: ValueAxis {
                min: 0
                max: 1
            }

            // Please note that month in JavaScript months are zero based, so 2 means March
            //XYPoint { x: toMsecsSinceEpoch(new Date()); y: 0.5 }
            //XYPoint { x: toMsecsSinceEpoch(new Date(1970, 0, 1)); y: 50 }
            //XYPoint { x: toMsecsSinceEpoch(new Date(1987, 12, 31)); y: 102 }
            //XYPoint { x: toMsecsSinceEpoch(new Date(1998, 7, 1)); y: 100 }
            //XYPoint { x: toMsecsSinceEpoch(new Date(2012, 8, 2)); y: 110 }
        }
    }
}

