import QtQuick.Controls
import Utils

MoreVertButton {
    id: menuBtn

    property bool isMenuOpened

    signal editClicked
    signal deleteClicked

    onClicked: if (!isMenuOpened) menu.open()

    Menu {
        id: menu
        onOpened: menuBtn.isMenuOpened = true
        onClosed: menuBtn.isMenuOpened = false
        y: parent.height
        height: 100
        rightMargin: 10

        MenuItem {
            onTriggered: editClicked()
            text: "Editar"
        }

        MenuItem {
            onTriggered: deleteClicked()
            text: "Eliminar"
        }
    }
}

