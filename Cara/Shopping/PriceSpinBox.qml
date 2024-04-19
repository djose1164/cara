import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int unitPrice
    property alias to: spinBox.to
    readonly property alias selectedQty: spinBox.value

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        Label {
            text: `RD$<b><font size="6">${root.unitPrice * spinBox.value}</font></b>`
            textFormat: Label.RichText
            horizontalAlignment: Label.AlignHCenter
            font.pointSize: 12
            Layout.fillWidth: true
        }

        SpinBox {
            id: spinBox
            from: 1
            Component.onCompleted: print("SpinBox's size: "+width+"x"+height)
        }
    }
}
