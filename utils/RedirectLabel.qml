import QtQuick
import QtQuick.Controls

Row {
    id: root

    property alias title: customerLbl.text
    property alias font: customerLbl.font

    signal clicked

    spacing: 14

    Label {
        id: customerLbl
        font.pointSize: 11
    }

    IconImage {
        source: "qrc:/icons/arrow_forward.svg"
        sourceSize: "24x24"
    }

    TapHandler {
        onTapped: clicked()
    }
}
