import QtQuick.Controls
import Cara.Controls

MoreVertButton {
    id: root

    property bool canDelete: false

    signal deleteClicked

    onClicked: menu.visible = !menu.visible

    Menu {
        id: menu
        y: parent.height
        rightMargin: 10

        MenuItem {
            height: visible ? implicitHeight : 0
            visible: root.canDelete
            onTriggered: deleteClicked()
            text: "Eliminar"
        }
    }
}

