import QtQuick
import QtQuick.Controls

Pane {
    id: refreshIcon
    width: parent.width
    height: 40
    visible: false

    Image {
        anchors.centerIn: parent
        source: "qrc:/icons/refresh_black_24dp.svg"
    }
}
