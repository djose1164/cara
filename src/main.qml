import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import "qrc:/users"

ApplicationWindow {
    id: app
    width: 360
    height: 640
    visible: true
    title: "Cara"

    property string iconSize: "24x24"
    readonly property bool previousLogin: settings.value("isLoggedIn") === "true"

    Component.onCompleted: {
        if (previousLogin)
            TheCurrentUser.init(createUserFromLocal());
        print("homePath: " + TheCurrentUser.homePath + "\n"
              + "userId: " + TheCurrentUser.userId);
    }

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: previousLogin ? "qrc:/main/MainPage.qml"
                                   : "qrc:/login/Login.qml"
    }

    Settings {
        id: settings
        category: "Credentials"
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    function createUserFromLocal() {
        return {
            "forename": settings.value("forename"),
            "surname": settings.value("surname"),
            "email": settings.value("email"),
            "userTypeId": settings.value("userTypeId"),
            "userId": settings.value("userId"),
        };
    }
}
