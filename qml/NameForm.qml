import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core
import "qrc:/utils"
import "qrc:/customers"
import "qrc:/singleton"
import "qrc:/main"

RowLayout {
    id: root

    property alias forename: forename.text
    property alias surname: surname.text

    TextField {
        id: forename
        placeholderText: "Nombre"
        Layout.fillWidth: true
    }

    TextField {
        id: surname
        placeholderText: "Apellido"
        Layout.fillWidth: true
    }
}
