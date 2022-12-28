import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/qr"

ToolBar {
    signal currentIndexChanged(int newIndex)


    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }


    RowLayout {
        anchors.fill: parent
        spacing: 20

        ToolButton {
            id: settingBtn
            background: Image {
                source: "qrc:/icons/menu-burger.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: sideMenu.open()
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 5
            Layout.leftMargin: 5
        }

        Label {
            id: homeLbl
            text: "Inicio"
            color: "black"
            font.pointSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
        }

        ToolButton {
            background: Image {
                antialiasing: false
                source: "qrc:/icons/qrcode-16x16.svg"
                sourceSize: iconSize
            }
            onClicked: qrCode.open()
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 5
            Layout.rightMargin: 5
        }
    }

    QR { id: qrCode }

    Component.onCompleted: currentIndexChanged.connect(updateHeader)

    function updateHeader(idx) {
        switch (idx) {
         case 0:
             homeLbl.text = "Inicio";
             break;
         case 1:
             homeLbl.text = "Buscar";
             break;
         case 2:
             homeLbl.text = "Nueva orden"
             break;
         case 3:
             homeLbl.text = "Facturas"
             break;
        }
    }
}
