import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"

RowLayout {
    id: root
    property alias quantity: quantityField.text
    readonly property alias price: productField.currentValue
    property alias currentText: productField.currentText
    required property int index
    required property int currentEntryIndex

    signal okClicked
    signal added(idx: int, entry: var)
    signal deleted(idx: int)

    width: ListView.view.width*.95
    spacing: width*.05
    ComboBox {
        id: productField
        Layout.fillWidth: true
        model: productModel.model
        textRole: "name"
        valueRole: "sell_price"
        currentIndex: 0
        onCurrentValueChanged: fetchEntry()
    }

    TextField {
        id: quantityField
        Layout.preferredHeight: productField.height
        placeholderText: "Cantidad"
        text: "1"
        inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
        onTextEdited: fetchEntry()
    }

    ProductRequest { id: productModel }

    Component.onCompleted: {
       okClicked.connect(fetchEntry);
        productModel.addSuggetionsToModel();
    }

    Component.onDestruction: deleted(currentEntryIndex);

    function fetchEntry() {
        const entryValue = {
            "product_id": findIn(productModel.model, item => item.name === productField.currentText).id,
            "quantity": quantity,
            "unit_price": price
        };
        added(currentEntryIndex, {entryValue});
    }
}
