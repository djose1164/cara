import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    property alias quantity: quantityField.text
    readonly property alias price: productField.currentValue
    property alias currentText: productField.currentText
    required property int index
    required property int currentEntryIndex

    signal okClicked
    signal entryAdded

    ComboBox {
        id: productField
        model: productModel
        textRole: "name"
        valueRole: "sell_price"
        currentIndex: 0
        onCurrentValueChanged: fetchEntry()
    }

    TextField {
        id: quantityField
        placeholderText: "Cantidad"
        text: "1"
        inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
        onTextEdited: fetchEntry()
    }

    Component.onCompleted: {
       okClicked.connect(fetchEntry)
    }

    Component.onDestruction: {
        orderModel.remove(currentEntryIndex);
        updateTotal();
    }
    function fetchEntry() {
        const entryValue = {
            "product_id": findIn(productModel, item => item.name === entry.currentText).id,
            "quantity": quantity,
            "unit_price": price
        };
        orderModel.set(currentEntryIndex, {"entryValue": entryValue});
        entryAdded();
        updateTotal();
    }
}
