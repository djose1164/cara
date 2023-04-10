import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias reviewText: reviewLbl.text
    readonly property StackLayout stackLayout: StackLayout.layout
    readonly property bool  hasText: reviewText.length

    signal editClicked
    signal deleteClicked

    Label {
        id: reviewLbl
        font.pointSize: 14
    }

    RoundButton {
        id: menuBtn
        property bool isMenuOpened
        icon.source: "qrc:/icons/more_vert.svg"
        flat: true
        onClicked: if (!isMenuOpened) menu.open()

        Menu {
            id: menu
            onOpened: menuBtn.isMenuOpened = true
            onClosed: menuBtn.isMenuOpened = false
            y: parent.height
            height: 100
            rightMargin: 10

            MenuItem {
                onTriggered: stackLayout.currentIndex = 0
                text: "Editar"
            }

            MenuItem {
                onTriggered: deleteClicked()
                text: "Eliminar"
            }
        }
    }
}
