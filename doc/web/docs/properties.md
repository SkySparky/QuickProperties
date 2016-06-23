Dealing with properties and timed properties 
============================

!!! note "2016 06 22: Documentation is not completely up to date" 
	Contact benoit@destrat.io if you have any problem using QuickProperties

## Installation ##

The recommended way of using QuickProperties is to include the library directly as a GIT submodule in your project:

```sh
# Install QuickProperties as a GIT submodule
$ git submodule add https://github.com/cneben/QuickProperties
& git submodule update
```

Once GIT has finished downloading, defaut project configuration file `common.pri` and `src/quickproperties2.pri` could be included directly in your main qmake .pro file:

```sh
# in your project main .pro qmake configuration file
include($$PWD/QuickProperties/common.pri)
include($$PWD/QuickProperties/src/quickproperties.pri)
```

By default, QuickProperties should be configured without Protocol Buffer, but it could be enabled if necessary in `common.pri`:

```sh
# Comment to remove Protocol Buffer serialization support
DEFINES += "QUICKPROPERTIES_HAVE_PROTOCOL_BUFFER"

# On win32, set the correct path to your protobuf install
```

From QML, the following import statements must be added (*qrc::QuickProperties2* is added automatially in your project ressources when including *quickproperties2.pri*):

```cpp
import "qrc:/QuickProperties2" as Qps
import QuickProperties 2.0 as Qps
```

## Managing properties ##

You should be familiar with the Qt property system before reading this document:

- Qt property system: [http://doc.qt.io/qt-5/properties.html](http://doc.qt.io/qt-5/properties.html)
- Qt dynamic properties documentation: [http://doc.qt.io/qt-5/qobject.html#dynamic-properties](http://doc.qt.io/qt-5/qobject.html#dynamic-properties)

Basically, a qps::Properties object is a regular QObject (or QtObject from a QML point of view) embedding standard Qt properties defined with the Q_PROPERTY macro or the QObject::setProperty() / QObject::property() methods. QuickProperties add an "abstract item model" (see [http://doc.qt.io/qt-5/model-view-programming.html](http://doc.qt.io/qt-5/model-view-programming.html)) on top of QObject to allow edition of property through  an MVC pattern. Using qps::TimedProperties instead of qps::Properties allow QuickProperties to track changes of properties value over time.

Properties are managed with the following QuickProperties classes:

- **qps::Properties**: FIXME \copybrief qps::Properties
- **qps::TimedProperties**: FIXME \copybrief qps::TimedProperties
- **qps::PropertiesList**: FIXME \copybrief qps::PropertiesList

QML components are more specifically designed to visualize the content of a qps::Properties object and edit its internal "property" elements:

- **Qps.PropertiesEditor**: FIXME \copybrief QpsPropertiesEditor
- **Qps.PropertyCreator**: FIXME \copybrief QpsPropertyCreator

## Creating properties ##

An empty qps::Properties object can be created with new and no arguments:

```cpp
  Properties* p1 = new qps::Properties( );
  // Or with timed values:
  //Properties* p1 = new qps::TimedProperties( );
  qDebug( ) << *p1;
```
results in the following output:
```cpp
Properties: ""
First date time stamp=QDateTime( Qt::TimeSpec(LocalTime))
Last date time stamp=QDateTime( Qt::TimeSpec(LocalTime))
objectName=""(current) 
no timed values.
label=""(current) 
no timed values.
current=""(current) 
no timed values.
first=""(current) 
no timed values.
last=""(current) 
no timed values.
timeValue="1"(current) 
no timed values.
propertiesModel=""(current) 
no timed values.
```

objectName is a QObject defined property and will appears whatever property where defined by the user in subclass. The following properties from 'label' to 'propertiesModel' are defined by qps::Properties, and are shown by default in QML properties view. Theses "systems" properties could be be hidden in the corresponding model using qps::Properties::hideStaticProperties() method.

## Adding a dynamic property ##

A dynamic property could be added with the standard QObject interface QObject::setProperty(), or using qps::Properties::addProperties() method. Once created, the "current" value can be set using QObject::setProperty() or qps::Properties::setProperty() methods:
```cpp
    Properties* p1 = new qps::Properties( );
    p1->addProperty( "intProperty", QVariant::fromValue< int >( 40 ) );
    p1->setProperty( "intProperty", 42 );
    qDebug( ) << *p1;
```
```cpp
"intProperty"="40"(current) 
```

If timed values support is necessary, create a qps::TimedProperties object instead of qps::Properties and specify a QDateTime time stamp in qps::TimedProperties::setProperty() method (QObject interface could not be used in that specific case or time values will not been taken into account):
```cpp
    Properties* p1 = new qps::TimedProperties( );
    p1->addProperty( "intProperty", QVariant::fromValue< int >( 40 ) );
    QDateTime current = QDateTime::currentDateTimeUtc( );
    p1->setProperty( "intProperty", current, 41 );
    p1->setProperty( "intProperty", current.addMSecs( 1000 ), 42 );
    qDebug( ) << *p1;
```
```cpp
"intProperty"="40"(current) 
	QDateTime(2015-07-29 16:05:05.363 UTC Qt::TimeSpec(UTC)):QVariant(int, 41)
	QDateTime(2015-07-29 16:05:06.363 UTC Qt::TimeSpec(UTC)):QVariant(int, 42)
```

** FIXME: Deprecated as of 20160621 **
	
Dynamic enumerations are supported with the qps::Properties::addEnumProperty(), qps::Properties::isEnum() and qps::Properties::getEnumValueLabels() methods. To add a dynamic property enum based on Qt Qt::PenStyle, use the following code:
```cpp
qps::Properties properties = new qps::Properties();
QStringList lineStyles; lineStyles << "No line" << "Solid line" << "Dash line" << "Dot line" << "Dash Dot line" << "Dash Dot Dot line";
properties->addEnumProperty( "Pen Style property", QVariant::fromValue< Qt::PenStyle >( Qt::SolidLine ), lineStyles );
```

## Managing static property elements ##

Static "property element" could be added with the standard Q_PROPERTY QObject mechanism, no specific calls to QuickProperties are necessary
```cpp
class MyProperties : public qps::Properties
{
    Q_OBJECT
public:
    MyProperties( QObject* parent = 0 ) : qps::Properties( parent, "myProperties" ), _doubleProperty( 0. ) { }
    virtual ~MyProperties( ) { }
private:
    Q_DISABLE_COPY( MyProperties )

    // User defined static double property
public:
    Q_PROPERTY( double  doubleProperty READ getDoubleProperty WRITE setDoubleProperty NOTIFY doublePropertyChanged )
    Q_INVOKABLE double  getDoubleProperty( ) const { return _doubleProperty; }
    Q_INVOKABLE void    setDoubleProperty( double dp ) { _doubleProperty = dp; emit doublePropertyChanged( ); }
signals:
    void        doublePropertyChanged( );
protected:
    double      _doubleProperty;
};
```

```cpp
MyProperties* mp = new MyProperties( this );
qDebug( ) << *mp;
```
Result in the following output:
```cpp
doubleProperty="0"(current) 
no timed values.
```

The QML version of this code is much simpler, properties are defined as with regular QtObject components (don't forget to call qmlRegisterType< qps::Properties >( "QuickProperties", 1, 0, "QpsProperties")) :
```cpp
import "qrc:/QuickProperties2" as Qps
import QuickProperties 2.0 as Qps

Qps.Properties {
  property real doubleProperty: 0.
}
```

Once defined, properties could be serialized to any XML stream and reused later with qps::Properties::serializeOut() and qps::Properties::serializeIn().

## Viewing and editing properties ##

Once defined, properties could be edited using the QpsPropertiesEditor, or just viewed with QpsPropertiesView, and used with regular binding to control other parts of your application. Using properties edition for building a simple style editor for a Rectangle component colors could be done easily in the following way:

```cpp
import "qrc:/QuickProperties2" as Qps
import QuickProperties 2.0 as Qps

Qps.Properties {
  id: rectangleStyle
  property color  color: "blue"
  property color  borderColor: "violet"
}
Rectangle {	// Our "styled" rectangle
  // x, y, width, height init...
  color:        rectangleStyle.color				// Bind Rectangle properties to our QpsProperties properties
  border.color: rectangleStyle.borderColor
  border.width: 1
}
Rectangle {
  // x: 15; y: 15; width: 250; height: 300	// Editor position init
  color: "lightblue"; border.width: 2; border.color: "black"
  Qps.PropertiesEditor {
    anchors.fill: parent; anchors.margins: 5
    properties: rectangleStyle      // QpsProperties is also an Qt item model, it could be used as a 'model' for any QML view control
  }
}
```

With more properties being defined in rectangleStyle, the result would look like:
![Properties edition](images/propsedit.png)

### Fine control over properties edition ###

Once exposed through a qps::Properties model, edition of specific "property elements" could be fine tuned  to restrict how  property should be modified by an end user. For example, in the previous Rectangle styling sample, the user should not be allowed to set a zero width or height. These limitations could be specified in declarative QML or C++ with the following classes (and their corresponding QML components QpsStringLimit, QpsRealLimit and QpsIntLimit):

- **qps::StringLimit**:  FIXME \copybrief qps::StringLimit
- **qps::RealLimit**:  FIXME \copybrief qps::RealLimit
- **qps::IntLimit**: FIXME \copybrief qps::IntLimit

Limits could be defined in plain declarative QML code (apply to corresponding Rectangle properties):
```cpp
QpsProperties {
    property string content: "Rectangle content string"
    QpsStringLimit { target: "content"; maxChars: 40 }		// String will not exceed 40 characters in QpsPropertiesEditor

    property real   radius: 5
    QpsRealLimit { target: "borderWidth"; min: 0.; max: 5.; stepSize:0.2 }

    property int    borderWidth: 3
    QpsIntLimit { target: "borderWidth"; min: 1; max: 15 }
}
```

In C++, limits could be declared in the following way (destruction via the parent QObject):
```cpp
// In .h
class MyProperties : public qps::Properties {
    //...
public:
	Q_PROPERTY( QString content READ getContent WRITE setContent NOTIFY contentChanged );
	Q_PROPERTY( double radius READ getRadius WRITE setRadius NOTIFY radiusChanged );	
	Q_PROPERTY( int borderWidth READ getBorderWidth WRITE setBorderWidth NOTIFY borderWidthChanged );
	//...
};
// In .cpp
MyProperties( QObject* parent = 0 ) :
    qps::Properties( parent ) //, ...
{
    new qps::StringLimit( this, "content", 40 );
    new qps::RealLimit( this, "radius", 0., 5., 0.2 );
    new qps::IntLimit( this, "borderWidth", 1, 15 ); // Will be destroyed from QObject parent destructor
}
```
With all properties referenced in 'target' parameter previously declared with Q_PROPERTY macros.



