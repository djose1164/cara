import QtQuick
import QtQuick.Controls
import "qrc:/utils"

Page {
    id: root
    title: "Inventario"
    header: TitleHeader {
        ToolButton {
            anchors.right: parent.right
            icon.source: "qrc:/icons/more_vert.svg"
        }
        title: root.title
    }
}
