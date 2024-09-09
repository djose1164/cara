import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls

Frame {
    id: root

    required property var model


    Material.elevation: 6

    GridLayout {
        id: gridLayout
        anchors {
            fill: parent
        }
        rowSpacing: 0
        columns: 2
        rows: 4

        Image {
            id: img
            source: model.imageUrl
            sourceSize: Qt.size(internal.imgSize, internal.imgSize)
            Layout.minimumHeight: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 300
            Layout.maximumHeight: 300
            Layout.rowSpan: 4
        }

        TrashButton {
            Layout.alignment: Qt.AlignRight
            onClicked: console.log("Delete")
        }

        RowLayout {
            //Layout.columnSpan: 2
            Layout.fillWidth: true
            Label {
                text: qsTr("RD$ " + internal.profitTotal)
                elide: Qt.ElideRight
                font.pointSize: 14
                font.bold: true
                Layout.fillWidth: true
            }
            SpinBox {
                id: spinBox
                from: 1
                to: 100 // Will it be necessary to update this limit?
                validator: IntValidator {}
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: implicitWidth * .77
                Layout.preferredHeight: implicitHeight * .77
                onValueChanged: model.selectedQty = spinBox.value
            }
        }

        // ToolSeparator {
        //     orientation: Qt.Horizontal
        //     Layout.fillWidth: true
        // }

        // Label {
        //     text: qsTr("Ganancia por c/u: RD$ " + internal.profitPerUnit)
        //     Layout.fillWidth: true
        // }

        Item {
            implicitHeight: 1
        }
    }

    QtObject {
        id: internal
        readonly property double profitPerUnit: model.price - model.buyPrice
        readonly property double profitTotal: profitPerUnit * model.selectedQty
        readonly property double imgSize: Math.min(root.width/2, root.height/2)
    }
}
