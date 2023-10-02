import QtQuick.Controls

RoundButton {
    id: menuBtn

    property bool isMenuOpened

    signal editClicked
    signal deleteClicked

    onClicked: if (!isMenuOpened) menu.open()

    icon.source: "qrc:/icons/more_vert.svg"
    flat: true

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

