import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.User

Pane {
    property int total

    signal payClicked

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: totalLbl
            Layout.alignment: Qt.AlignHCenter
            text: `Total: <b>RD$${total}</b>`
            font.pointSize: 15
        }

        RoundButton {
            text: "Ordenar"
            Layout.fillWidth: true
            Material.background: Material.Green
            Material.foreground: "white"
            onClicked: payClicked()
        }
    }
}
