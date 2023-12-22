import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import User

Pane {
    property int total
    readonly property bool userIsGuest: userStore.user.userType === UserEntity.Guest
    signal payClicked

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: totalLbl
            Layout.alignment: Qt.AlignHCenter
            text: `Total: RD$${total}`
            font.pointSize: 12
        }

        RoundButton {
            onClicked: userIsGuest ? dialog.open() : payClicked()

            text: "Pagar"
            Layout.fillWidth: true
//            Material.background: Material.Green
//            Material.foreground: "white"
        }
    }

    LocalUserStore { id: userStore }

    MessageDialog {
        id: dialog
        onButtonClicked: close()
        text: "Usuario no registrado"
        informativeText: "Para poder realizar compras necesitas estar registrado."
        buttons: MessageDialog.Ok
    }
}
