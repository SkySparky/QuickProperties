Exposing containers of QObject to QML 
============================

## Introduction ##

```cpp
    using QObjects = qps::ContainerListModel< QVector, QObject* >;
    QObjects objects;
    QObject* o = new QObject();
    o->setObjectName( "Hola" );

    objects.append( o );
    objects.append( new QObject() );
    objects.append( new QObject() );
    objects.append( nullptr );  // Shouldn't throw
    
	EXPECT_EQ( objects.rowCount(), 3 );
    EXPECT_EQ( objects.getItemCount(), 3 );
    EXPECT_EQ( objects.at(0)->objectName(), "Hola" );
```




