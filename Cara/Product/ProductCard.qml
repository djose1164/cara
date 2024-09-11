import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.User
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
            from: UserSettings.userTypeId === UserEntity.ADMIN ? 0 : 1
            editable: true
            inputMethodHints: Qt.ImhDigitsOnly
        }
    }

    Binding {
        target: spinBox
        property: "down.indicator"
        when: spinBox.value === 1
        value: Item {
            width: spinBox.height
            height: width

            ItemDelegate {
                id: inId
                x: spinBox.spacing
                y: spinBox.spacing
                width: parent.width - 2 * spinBox.spacing
                height: parent.height - 2 * spinBox.spacing
                contentItem: Item {
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/icons/trash-can-outline.svg"
                        sourceSize: Qt.size(20, 20)
                    }
                }

                onClicked: root.deleted()
            }
        }
    }
}
