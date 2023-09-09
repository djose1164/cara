import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils" as Utils
import "qrc:/main"
import "qrc:/singleton"

Page {
    id: root

    property var pageHeaderView
    readonly property StackView myStack: StackView.view
    property var product
    property var stocks
    property int inventoryId
    property string mode: "add"

    header: Utils.TitleHeader {
        title: "Agregar nuevo producto"
        onBackClicked: myStack.pop()
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 1

        BusyIndicator {}

        Item {
            ColumnLayout {
                anchors.centerIn: parent
                width: root.width*.8
                spacing: 10

                Label { text: "Nombre" }

                TextField {
                    id: productName
                    text: product.name
                    placeholderText: "Nombre del producto"
                    Layout.fillWidth: true
                }

                Label {
                    text: "Descripcion"
                    visible: descriptionField.text
                }

                TextField {
                    id: descriptionField
                    text: product.description
                    placeholderText: "Descripcion del producto"
                    Layout.fillWidth: true
                }

                Label { text: priceBuy.placeholderText }

                TextField {
                    id: priceBuy
                    text: product.buy_price
                    placeholderText: "Precio de compra"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Label { text: priceSell.placeholderText }

                TextField {
                    id: priceSell
                    text: product.sell_price
                    placeholderText: "Precio de venta"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Label { text: stockField.placeholderText }

                TextField {
                    id: stockField
                    Layout.fillWidth: true
                    text: stocks.in_stock
                    placeholderText: "Cantidad"
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    text: mode === "add" ? "Agregar" : "Guardar"
                    Layout.fillWidth: true
                    Material.background: Material.Green
                    Material.foreground: "white"
                    onClicked: {
                        updateInventory();
                        const body = {
                            inventoryId,
                            product,
                            stocks,
                        };
                        stackLayout.currentIndex = 0;
                        if (mode === "add")
                        productRequest.addProduct(productName.text, description.text, priceBuy.text, priceSell.text);
                        else if(mode === "edit")
                            productRequest.editProduct(body);
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog
        text: mode === "add" ? "Producto guardado." : "Producto editado."
        informativeText: "Puedes chequear el producto desde Inventario.";
        onButtonClicked: close()
    }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {
            ProductManager.updateModel();
            stackLayout.currentIndex = 1;
            messageDialog.open();
        }

        request.onFailed: resp => {
            messageDialog.text = "No se ha podido guardar el producto.";
            messageDialog.informativeText = resp.message;
            messageDialog.open();
            stackLayout.currentIndex = 1;
        }
    }

    function updateInventory() {
        product.name = productName.text;
        product.description = descriptionField.text;
        product.buy_price = priceBuy.text;
        product.sell_price = priceSell.text;
        delete product.id;

        stocks.in_stock = stockField.text;
        delete stocks.id;
    }
}
