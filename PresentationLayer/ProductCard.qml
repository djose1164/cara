import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GridLayout {
    id: root

    property string imageUrl
    property string productName
    property int productPrice
    property int productId
    property int productQuantity
    property int total

    readonly property alias spinBox: spinBox

    readonly property var product: {
        "id": productId,
        "quantity": productQuantity
    }

    signal deleted

    width: ListView.view.width
    height: 100

    rows: 2
    columns: 3

    ProductImage {
        Layout.rowSpan: 2
        source: imageUrl
        sourceSize: "100x100"
    }

    Label {
        text: productName
        font.pointSize: 13
        font.bold: true

    }

    Label {
        text: `RD$${total}`
        Layout.column: 1
        Layout.row: 1
    }

    RoundButton {
        onClicked: deleted();
        Layout.column: 2
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
        Layout.column: 2
        Layout.row: 1
        from: 1
        editable: true
        Keys.onReturnPressed: Qt.inputMethod.hide()
    }
}
