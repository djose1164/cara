import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils" as Utils
import "qrc:/main"

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
                    text: "Ok"
                    Layout.alignment: Qt.AlignHCenter
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
        text: productRequest.success ? "Producto guardado." : "No se ha podido guardar el producto."
        informativeText: {
            if (!productRequest.success)
                productRequest.message;
        }
    }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {
            messageDialog.open();
            stackLayout.currentIndex = 1;
        }
    }
}
