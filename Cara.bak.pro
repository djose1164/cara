QT += qml quick core network svg quickcontrols2

SOURCES += \
    src/core/app.cpp \
    src/core/auth.cpp \
    src/core/billbusiness.cpp \
    src/core/cartbusiness.cpp \
    src/core/constants.cpp \
    src/core/loginmanager.cpp \
    src/core/paymentbusiness.cpp \
    src/entities/billentity.cpp \
    src/entities/customerentity.cpp \
    src/entities/detailsentity.cpp \
    src/entities/inventory.cpp \
    src/entities/orderstatus.cpp \
    src/entities/paymententity.cpp \
    src/entities/productentity.cpp \
    src/entities/orderentity.cpp \
    src/entities/userentity.cpp \
    src/models/billmodel.cpp \
    src/models/billproxymodel.cpp \
    src/models/customermodel.cpp \
    src/models/ordermodel.cpp \
    src/models/orderstatusproxymodel.cpp \
    src/qml/currentuser.cpp \
    src/qml/main.cpp \
    src/data/rest.cpp \


RESOURCES += \
    src/qml/qml.qrc \
    src/assets/res.qrc

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
    src/core/app.h \
    src/core/auth.h \
    src/core/billbusiness.h \
    src/core/cartbusiness.h \
    src/core/constants.h \
    src/core/loginmanager.h \
    src/core/paymentbusiness.h \
    src/entities/billentity.h \
    src/entities/customerentity.h \
    src/entities/detailsentity.h \
    src/entities/inventory.h \
    src/entities/orderstatus.h \
    src/entities/paymententity.h \
    src/entities/productentity.h \
    src/entities/userentity.h \
    src/entities/orderentity.h \
    src/entities/jsonexportable.h \
    src/models/billmodel.h \
    src/models/billproxymodel.h \
    src/models/customermodel.h \
    src/models/ordermodel.h \
    src/models/orderstatusproxymodel.h \
    src/qml/currentuser.h \
    src/data/rest.h

INCLUDEPATH += \
    src/qml/ \
    src/data/ \
    src/core/ \
    src/entities/ \
    src/models/


DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml


