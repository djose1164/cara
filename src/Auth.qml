import QtQuick
import QtCore
import "qrc:utils"

Item {
    id: root
    property alias request: request
    property alias credentials: settings

    property bool authenticated: request.success

    Request {
        id: request
        source: "qrc:/js/login.mjs"
    }

    Settings {
        id: settings
        category: "Credentials"
    }

    function authenticate(entity) {
        request.send(entity);
    }
}
