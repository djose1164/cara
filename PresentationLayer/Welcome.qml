import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string fullName

    width: parent.width
    height: 50

    Label {
        id: label
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        text: `¡Bienvenido${fullName}, mira lo que tenemos disponible para ti!`
        font.pointSize: 14
        wrapMode: Text.WordWrap
    }
}
