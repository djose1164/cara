import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils" as Utils
import "qrc:/main"
import "qrc:/singleton"

Page {
    property var pageHeaderView
    readonly property StackView myStack: StackView.view

    header: Utils.TitleHeader {
        stackView: myStack
        title: "Agregar nuevo producto"
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 1

        BusyIndicator {}

        Item {
            ColumnLayout {
                anchors.centerIn: parent
                width: parent.width * .7
                height: width * 1.1
                spacing: 10

                Component.onCompleted: print("width: " + width + "\theight: " + height)

                TextField {
                    id: productName
                    placeholderText: "Nombre del producto"
                    Layout.fillWidth: true
                }

                TextField {
                    id: description
                    placeholderText: "Descripcion del producto"
                    Layout.fillWidth: true
                }

                TextField {
                    id: priceBuy
                    placeholderText: "Precio de compra"
                    Layout.fillWidth: true
                }

                TextField {
                    id: priceSell
                    placeholderText: "Precio de venta"
                    Layout.fillWidth: true
                }

                Button {
                    text: "Agregar"
                    Layout.fillWidth: true
                    Material.background: Material.Green
                    Material.foreground: "white"
                    onClicked: {
                        stackLayout.currentIndex = 0;
                        productRequest.addProduct(productName.text, description.text, priceBuy.text, priceSell.text);
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog
        onButtonClicked: close()
    }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {
            ProductManager.updateModel();

            messageDialog.text = "Producto guardado.";
            messageDialog.informativeText = "Puedes chequear el producto desde Inventario.";
            messageDialog.open();
            stackLayout.currentIndex = 1;
        }
        request.onFailed: resp => {
            messageDialog.text = "No se ha podido guardar el producto.";
            messageDialog.informativeText = resp.message;
            messageDialog.open();
            stackLayout.currentIndex = 1;
        }
    }
}
