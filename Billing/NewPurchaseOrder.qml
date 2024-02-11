import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Customer
import User
import Utils
import Inventory
import Product
import entities
import Billing
import Salesperson

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    header: ToolBar {
        Label {
            anchors.centerIn: parent
            text: "Facturacion"
            font.pointSize: 16
            font.bold: true
        }
    }

    states: [
        State {
            name: "creating"
            PropertyChanges {
                target: busyPopup
                visible: true
            }
            PropertyChanges {
                target: acceptBtn
                enabled: false
            }
        },
        State {
            name: "orderCreated"
            PropertyChanges {
                target: busyPopup
                visible: false
            }
        }
    ]

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight
        ColumnLayout {
            id: columnLayout
            width: root.width * .95
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30

            Loader {
                id: customerLoader
                Layout.fillWidth: true
                Layout.topMargin: 10
                sourceComponent: RedirectLabel {
                    id: customerRedirect
                    title: "Elegir cliente"
                    font.pointSize: 14
                    onClicked: customerSelector.open()
                }
            }

            CustomerSelector {
                id: customerSelector
                readonly property string delegateSource: "qrc:/qt/qml/Customer/SelectedCustomer.qml"
                width: root.width
                height: root.height
                onCustomerSelected: model => {
                    const customerId = model.customerId;
                    customerLoader.setSource(delegateSource, {
                        fullname: model.fullname,
                        stackView: root.stackView,
                        customerId,
                        customerSelector
                    });
                }
            }

            StackLayout {
                id: stackLayout
                currentIndex: thisSelectedProducts.count > 0 ? 1 : 0
                Layout.minimumHeight: {
                    if (currentIndex === 0)
                        return productRedirect.implicitHeight;
                    const count = selectedProductsView.model.count;
                    const itemHeight = 160;
                    if (count === 1)
                        return itemHeight;
                    else if (count === 2)
                        return itemHeight * 1.65;
                    else
                        return itemHeight * 2;
                }

                RedirectLabel {
                    id: productRedirect
                    title: "Agregar producto(s)"
                    font.pointSize: 14
                    Layout.maximumHeight: implicitHeight
                    onClicked: productSelector.open()
                }

                SelectedProducts {
                    id: selectedProductsView
                    model: thisSelectedProducts
                    onRedirectClicked: productSelector.open()
                    onDeleteClicked: (index, productId) => {
                        productSelector.removeSelectionOf(productId);
                        thisSelectedProducts.remove(index);
                    }
                }
            }

            PaymentInput {
                id: paymentInput
                calculator: costCalculator
                Layout.fillWidth: true
            }

            RedirectLabel {
                id: dateRedirectLbl
                title: datePicker.selectedDate.toLocaleDateString()
                font.pointSize: 14
                onClicked: datePicker.open()
            }

            Button {
                id: acceptBtn
                text: "Aceptar"
                Layout.fillWidth: true
                Layout.bottomMargin: 10
                onClicked: acceptHandle()
            }
        }
    }

    DatePicker {
        id: datePicker
        width: root.width * .9
        height: root.height * .75
        onAccepted: print(datePicker.selectedDate.toLocaleDateString())
    }

    ProductSelector {
        id: productSelector
        width: app.width
        height: app.height
        salespersonId: SalespersonSettings.salespersonId
        selectedProducts: thisSelectedProducts
    }

    ProductModel { id: thisSelectedProducts }

    CostCalculator {
        id: costCalculator
        model: thisSelectedProducts
    }

    LocalUserStore { id: userStore }

    MessageDialog {
        id: message
        onButtonClicked: close()
    }

    OrderController {
        id: orderController
        onOrderCreated: {
            root.state = "orderCreated";
            message.title = "Exito al crear orden";
            message.informativeText = "La orden ha sido adjuntada al cliente "
                    + "exitosamente. Puedes verla en el area de 'Clientes'";
            message.open();
        }
    }

    Popup {
        id: busyPopup
        width: root.width * .8
        anchors.centerIn: Overlay.overlay
        RowLayout {
            anchors.fill: parent

            BusyIndicator {
                running: busyPopup.visible
            }

            Label {
                text: "Procesando..."
                font.pointSize: 12
            }
        }
    }

    function acceptHandle() {
        try {
            validate();
            const customerItem = customerLoader.item;
            for (let i = 0; i < thisSelectedProducts.count; ++i)
                print(thisSelectedProducts.get(i));
            const orderJson = {
                salesperson: { id: SalespersonSettings.salespersonId },
                customer: customerItem.toJson(),
                products: selectedProductsView.toJson(),
                pay: paymentInput.toJson(),
                orderDate: datePicker.toShortDate()
            };
            print(`orderJson: ${JSON.stringify(orderJson, null, 2)}`);
            orderController.createOrder(orderJson);
            root.state = "creating";
        } catch (err) {
            print("NewPurchaseOrder: "+err);
            message.title = "Error al crear orden"
            message.informativeText = err;
            message.open();
        }
    }

    function validate() {
        if (!(customerLoader.item instanceof SelectedCustomer))
            throw "Necesitas elegir un cliente";
        else if (costCalculator.total === 0)
            throw "Debes agregar al menos un producto a la orden";
        paymentInput.validate();
    }
}
