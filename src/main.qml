import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings

ApplicationWindow {
    width: 340
    height: 680
    visible: true
    title: "Cara"

    property string source
    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: source
    }

    Settings {
        id: settings
        category: "Credentials"
        property string username
        property string password

        Component.onCompleted: {
            let username = settings.value("username", false);
            if (username && settings.value("password", false))
                mainStackView.replace("qrc:/home/Home.qml", {"currentUser": username});
            else
                mainStackView.replace("qrc:/Login.qml");
        }
    }
}
