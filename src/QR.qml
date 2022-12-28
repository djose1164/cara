import QtQuick
import QtQuick.Controls
import "qrc:/utils" as Utils

Utils.CaDrawer {
    width: parent.width
    height: parent.height
    edge: Qt.BottomEdge
    title: "Codigo QR"

    TextField {
        anchors.top: parent.top
        anchors.topMargin: 20
        id: inputField
        text: "Hello world!"
    }


    Image{
        anchors.top: inputField.bottom
        anchors.topMargin: 20
        source: "image://QZXing/encode/" + inputField.text +
                "?correctionLevel=M" +
                "&format=qrcode"
        sourceSize: "100x100"
    }

}
