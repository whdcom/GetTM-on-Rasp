QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#DEFINES += UBUNTU
if(contains(DEFINES, UBUNTU)) {
    unix:!macx: LIBS += -L$$PWD/../../bin/ubuntu/Release/PortingMod/ -lPortingMod

    INCLUDEPATH += $$PWD/../../include
    DEPENDPATH += $$PWD/../../include

    unix:!macx: PRE_TARGETDEPS += $$PWD/../../bin/ubuntu/Release/PortingMod/libPortingMod.a


    unix:!macx: LIBS += -L$$PWD/../../../../3rdLib/libusb-1.0.21-ubuntu/install_64/lib/ -lusb-1.0

    INCLUDEPATH += $$PWD/../../../../3rdLib/libusb-1.0.21-ubuntu/install_64/include/libusb-1.0
    DEPENDPATH += $$PWD/../../../../3rdLib/libusb-1.0.21-ubuntu/install_64/include/libusb-1.0
    DESTDIR = $$PWD/../bin/ubuntu/Release/test

}


DEFINES += RaspberryPi
if(contains(DEFINES, RaspberryPi)) {
    unix:!macx: LIBS += -L$$PWD/-lModuleSDK

    INCLUDEPATH += $$PWD/../../include
    DEPENDPATH += $$PWD/../../include

    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/libModuleSDK.so.2.1.2

    unix:!macx: LIBS += -L$$PWD/-lPortingMod

    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/libPortingMod.so.1.0.0
    DESTDIR = $$PWD/../bin/RaspberryPi/Release/test


    unix:!macx: LIBS += -L$$PWD/../../../../../../usr/lib/-lusb-1.0

    INCLUDEPATH += $$PWD/../../../../../../usr/lib/libusb-1.0
    DEPENDPATH += $$PWD/../../../../../../usr/lib/libusb-1.0

    unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/libusb-1.0.so.0.1.0


}

