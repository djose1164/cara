import QtQuick
import QtQuick.Controls
import Qt.labs.settings

ApplicationWindow {
    width: 360
    height: 640
    visible: true
    title: "Cara"

    property string source
    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: BusyIndicator {}
    }

    Settings {
        id: settings
        category: "Credentials"
        property string username
        property string password

        Component.onCompleted: {
            const username = settings.value("username", false);
            const password = settings.value("password", false);
            print(username + " " + password);
            if (username && password)
                loginWorker.sendMessage({"username": username, "password": password});
            else
                mainStackView.replace("qrc:/main/Login.qml");
        }
    }

    WorkerScript {
        id: loginWorker
        source: "qrc:/js/login.mjs"
        onMessage: (response) => response.code === "success" ?
                       mainStackView.replace("qrc:/home/Home.qml", {
                                                 "currentUser": settings.value("username", false)
                                             })
                     : mainStackView.replace("qrc:/main/Login.qml")
    }
}
