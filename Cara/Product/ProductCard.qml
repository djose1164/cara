pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls

Rectangle {
    id: root

    property string imageUrl
    property string productName
    property int productPrice
    property int productId
    property int total
    property int index

    readonly property alias spinBox: spinBox

    signal deleted(index: int, productId: int)

    width: 400
    height: 100
    radius: 10
    border.width: 1

    BusyStackLayout {
        id: imageStackLayout
        width: 100
        height: 100
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        ProductImage {
            onReady: imageStackLayout.currentIndex = 1
            source: root.imageUrl
            sourceSize: Qt.size(100, 100)
        }
    }

    GridLayout {
        anchors.left: imageStackLayout.right
        anchors.leftMargin: 5
        anchors.right: root.right
        height: root.height
        rows: 2
        columns: 2

        Label {
            text: root.productName
            font.pointSize: 13
            font.bold: true
            wrapMode: Label.Wrap
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Label {
            text: `RD$${root.total}`
            Layout.row: 1
            Layout.column: 0
        }

        SpinBox {
            id: spinBox
            Layout.preferredWidth: 105
            Layout.preferredHeight: 45
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 5
            Layout.bottomMargin: 5
            //from: 1
            editable: true
            inputMethodHints: Qt.ImhDigitsOnly

            down.indicator: DownIndicator {
                enabled: !parent.enabled
                spacing: spinBox.spacing
                shouldDelete: spinBox.value === 1
            }
            Component.onCompleted: if (spinBox.value === 0) spinBox.increase()
        }
    }

    component DownIndicator : ToolButton {
        id: downIndicator

        property bool shouldDelete

        property alias color: rectangle.color

        x: spinBox.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40
        color: (hovered || pressed ) ? spinBox.Material.listHighlightColor : "white"
        background: Rectangle {
            color: downIndicator.pressed ? spinBox.Material.listHighlightColor: "transparent"
            radius: downIndicator.width
        }

        Rectangle {
            id: rectangle
            x: downIndicator.spacing
            y: downIndicator.spacing
            width: downIndicator.width - (downIndicator.spacing * 2)
            height: downIndicator.height - (downIndicator.spacing * 2)
        }

        icon.source: downIndicator.shouldDelete ? "qrc:/icons/trash-can-outline.svg"
                                                : "remove.svg"
        icon.width: 24
        icon.height: 24
        autoRepeat: true
        flat: true
        onClicked: {
            if (downIndicator.shouldDelete)
                root.deleted(root.index, root.productId)
            else
                spinBox.decrease()
        }
    }
}
