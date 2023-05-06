import QtQuick.Controls
import QtQuick.Layouts

Pane {
    property int total

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
            onClicked: payClicked()

            text: "Pagar"
            Layout.fillWidth: true
            Material.background: Material.Green
            Material.foreground: "white"
        }
    }
}
