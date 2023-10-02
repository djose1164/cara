import QtQuick
import QtQuick.Controls
import "qrc:/singleton"

Item {
    Label {
        width: parent.width * .8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: height/2
        wrapMode: Label.WordWrap
        text: TheCurrentUser.isUser && !TheCurrentUser.isCostumer
              ? "Aun no haz realizado ninguna orden."
              : "Necesitas registrarte para poder ver tus ordenes recientes."
        font.pointSize: 15
        horizontalAlignment: Label.AlignHCenter
    }
}
