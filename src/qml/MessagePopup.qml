import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popup
    width: parent.width - 20
    anchors.centerIn: parent
    focus: true
    modal: true

    property alias text: popupLbl.text

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            id: popupLbl
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            font.pointSize: 20
        }

        Button {
            text: "Ok"
            Layout.alignment: Qt.AlignHCenter
            onClicked: popup.close()
        }
    }
}
