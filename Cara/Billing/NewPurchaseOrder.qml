pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Customer
import Cara.User
import Cara.Controls
import Cara.Billing
import Cara.Utils
import Cara.ProductInventory

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
                busyPopup.visible: true
                acceptBtn.enabled: false
            }
        },
        State {
            name: "orderCreated"
            PropertyChanges {
                busyPopup.visible: false
            }
        }
    ]

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.margins: Math.max(scrollView.effectiveScrollBarWidth, 10)
            spacing: 30

            ChooseCustomer {
                id: chooseCustomer
                Layout.fillWidth: true
                onSelectedCustomerClicked: c => stackView.pushItem("qrc:/qt/qml/Cara/Customer/CustomerPage.qml",
                                                                   {customerId: c.customerId,
                                                                    model: c})
            }

            ProductSale {
                id: productSale
                selectorModel: ProductInventoryController.inventory
                Layout.fillWidth: true
            }

            PaymentInput {
                id: paymentInput
                calculator: costCalculator
                Layout.fillWidth: true
            }

            RedirectLabel {
                id: dateRedirectLbl
                title: datePicker.isValid ? datePicker.dateString
                                          : "Seleccionar fecha"
                Layout.fillWidth: true
                onClicked: datePicker.open()
            }

            Button {
                id: acceptBtn
                text: "Aceptar"
                Layout.fillWidth: true
                Layout.bottomMargin: 10
                onClicked: acceptHandle()
            }

            Item {}
        }
    }

    ProductPriceTotaler {
        id: costCalculator
        model: productSale.productSelectionModel
        role: ProductPriceTotaler.SellPriceRole
    }

    DatePicker {
        id: datePicker
        width: root.width * .9
        height: root.height * .75
        onAccepted: print(datePicker.selectedDate.toLocaleDateString())
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            if (!internal.canPrint)
                return;
            pdfPrinter.printBill();
            root.state = "creating";
        }
    }

    OrderController {
        id: orderController
        onOrderCreated: newOrder => {
            root.state = "orderCreated";
            message.title = "Exito al crear orden";
            message.informativeText = "¿Deseas exportar la factura en PDF?";
            message.open();
            orderTemplate.order = newOrder.order;
            productSale.refreshSelector();
            ProductInventoryController.updateSellerInventory(UserSettings.sellerId);
        }
        onErrorOccurred: {
            errorDialog(errorString);
            root.state = "";
        }
    }

    BillPrinter {
        id: pdfPrinter
        document: OrderTemplate {
            id: orderTemplate
        }
        onBillPrinted: path => {
            root.state = "";
            ShareUtils.share("file://" + path);
        }
    }

    ProcessingIndicator {
        id: busyPopup
        width: root.width * .8
    }

    QtObject {
        id: internal
        readonly property bool canPrint: root.state === "orderCreated"
    }

    function acceptHandle() {
        try {
            validate();
            const orderJson = {
                salesperson: { id: UserSettings.sellerId },
                customer: chooseCustomer.toJson(),
                products: productSale.toJson(),
                pay: paymentInput.toJson(),
                orderDate: datePicker.toShortDate()
            };
            print(`orderJson: ${JSON.stringify(orderJson, null, 2)}`);
            if (!orderController.validateOrder(orderJson))
                throw orderController.errorString;

            orderController.createOrder(orderJson);
            root.state = "creating";
        } catch (err) {
            print("NewPurchaseOrder: "+err);
            errorDialog(err);
        }
    }

    function validate() {
        chooseCustomer.validate();
        productSale.validate();
        productSale.validateStocks();
        paymentInput.validate();
    }

    function errorDialog(err: string) {
        message.title = "Error al crear orden"
        message.informativeText = err;
        message.open();
    }
}
