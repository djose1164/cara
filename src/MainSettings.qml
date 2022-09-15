import QtQuick
import Qt.labs.settings

Item {
    // TODO: Improve start.
    Settings {
        id: settings
        category: "Credentials"
        property string username
        property string password

        Component.onCompleted: {
            const username = settings.value("username", false);
            const password = settings.value("password", false);

            if (username && password)
                mainStackView.replace("qrc:/main/MainPage.qml", {
                                          "currentUser": settings.value("username", false)
                                      })
            else
                mainStackView.replace("qrc:/main/Login.qml");
        }
    }


}
