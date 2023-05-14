import QtQuick
import QtQuick.Controls

Item {
    id: root

    Label {
        anchors.fill: parent
        text: "El carrito esta vacio, intenta agregar algunos producto."
        font.pointSize: 16
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        wrapMode: Label.Wrap
    }
}
