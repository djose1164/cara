QT += qml quick core network svg quickcontrols2 websockets

SOURCES += \
    BusinessLayer/auth.cpp \
    BusinessLayer/cartbusiness.cpp \
    EntityLayer/orderdetailsentity.cpp \
    EntityLayer/orderentity.cpp \
    EntityLayer/paymententity.cpp \
    EntityLayer/productentity.cpp \
    EntityLayer/userentity.cpp \
    PresentationLayer/currentuser.cpp \
    PresentationLayer/main.cpp \
    DataLayer/rest.cpp \


RESOURCES += \
    PresentationLayer/qml.qrc \
    assets/res.qrc

CONFIG += c++20 qml console qml_debug qmltypes

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

QML_IMPORT_NAME = com.cara
QML_IMPORT_MAJOR_VERSION = 1

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    BusinessLayer/auth.h \
    BusinessLayer/cartbusiness.h \
    EntityLayer/orderdetailsentity.h \
    EntityLayer/orderentity.h \
    EntityLayer/paymententity.h \
    EntityLayer/productentity.h \
    EntityLayer/userentity.h \
    PresentationLayer/currentuser.h \
    DataLayer/rest.h

INCLUDEPATH += \
    PresentationLayer/ \
    DataLayer/ \
    BusinessLayer/ \
    EntityLayer/


DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
android: include(/home/djose1164/Android/Sdk/android_openssl/openssl.pri)

contains(ANDROID_TARGET_ARCH,x86_64) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
