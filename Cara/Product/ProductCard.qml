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

    readonly property alias spinBox: spinBox

    signal deleted

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
            from: 0
            editable: true
            inputMethodHints: Qt.ImhDigitsOnly

            down.indicator: DownIndicator {
                x: spinBox.mirrored ? parent.width - width : 0
                width: height
                height: spinBox.height
                spacing: spinBox.spacing
                shouldDelete: spinBox.value === 1
                color: spinBox.down.hovered ? spinBox.Material.listHighlightColor : "white"
            }

            onValueChanged: if (spinBox.value === 0) root.deleted()
        }
    }

    component DownIndicator : Item {
        id: downIndicator

        property bool shouldDelete
        property int spacing
        property bool highlighted
        property alias color: rectangle.color

        implicitWidth: 40
        implicitHeight: 40

        Rectangle {
            id: rectangle
            x: downIndicator.spacing
            y: downIndicator.spacing
            width: downIndicator.width - (downIndicator.spacing * 2)
            height: downIndicator.height - (downIndicator.spacing * 2)

            Image {
                anchors.centerIn: parent
                source: downIndicator.shouldDelete ? "qrc:/icons/trash-can-outline.svg"
                                                   : "remove.svg"
                sourceSize: Qt.size(24, 24)
            }
        }
    }
}
