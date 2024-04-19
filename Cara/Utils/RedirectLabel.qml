import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property alias title: customerLbl.text
    property alias font: customerLbl.font

    signal clicked

    implicitWidth: customerLbl.implicitWidth + icon.implicitWidth
    implicitHeight: Math.max(customerLbl.implicitHeight, icon.implicitHeight)

    RowLayout {
        anchors.fill: parent

        Label {
            id: customerLbl
            wrapMode: Label.Wrap
            elide: Label.ElideRight
            font.pointSize: 12
        }

        IconImage {
            id: icon
            source: "qrc:/icons/arrow_forward.svg"
            sourceSize: "24x24"
            Layout.alignment: Qt.AlignRight
        }

        TapHandler {
            onTapped: clicked()
        }
    }
}
