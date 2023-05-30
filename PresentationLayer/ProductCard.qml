import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara

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

    ProductImage {
        id: productImage
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.verticalCenter: root.verticalCenter
        source: imageUrl
        sourceSize: "100x100"
    }

    GridLayout {
        anchors.left: productImage.right
        anchors.leftMargin: 10
        anchors.right: root.right
        height: root.height
        rows: 2
        columns: 2

        Label {
            text: productName
            font.pointSize: 13
            font.bold: true

        }

        Label {
            text: `RD$${total}`
            Layout.row: 1
            Layout.column: 0
        }

        RoundButton {
            onClicked: deleted();
            Layout.column: 1
            Layout.row: 0
            Layout.alignment: Qt.AlignTrailing
            IconImage {
                anchors.centerIn: parent;
                source: "qrc:/icons/trash-can-outline.svg"
            }
            flat: true
        }

        SpinBox {
            id: spinBox
            Layout.preferredWidth: 128
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignRight
            from: CurrentUser.userType === CurrentUser.ADMIN ? 0 : 1
            editable: true
            Keys.onReturnPressed: Qt.inputMethod.hide()
        }
    }
}
