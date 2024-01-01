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
import "qrc:/utils"
import "qrc:/product"
import "qrc:/payment"

ScrollablePage {
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
    columnLayout {
        spacing: 20
    }
    scrollView.contentWidth: scrollView.availableWidth

    Item { height: 1 }

    Loader {
        id: customerLoader
        Layout.fillWidth: true
        // Layout.fillHeight: true
        onHeightChanged: print("loader height: "+height)
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
        salespersonId: userStore.user.salespersonId
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
        onClicked: datePicker.open()
    }

    Button {
        text: "Aceptar"
        Layout.fillWidth: true
        onClicked: acceptHandle()
    }

    Item { height: 1 }

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
        salespersonId: userStore.user.salespersonId
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

    function acceptHandle() {
        try {
            validate();
        } catch (err) {
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
        else if (paymentInput.checkState === Qt.Unchecked)
            throw "Debes selecionar la condicion de pago de la orden";
        else if(paymentInput.selectedStatus === PaymentEntity.Credited && paymentInput.value < 1)
            throw "Debes digitar un monto valido mayor a 0";
    }
}
