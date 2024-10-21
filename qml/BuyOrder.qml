import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Salesperson
import Cara.User
import Cara.Billing
import Cara
import Cara.Controls
import Cara.ProductInventory
import "qrc:/orders"


Page {
    id: root

    readonly property T.StackView myStack: StackView.view
    readonly property var salesperson: SalespersonSettings.entity()

    title: "Ordenes de Compra"
    header: TitleHeader {
        title: root.title
        onBackClicked: myStack.pop()
    }

    Component.onCompleted: {
        spController.readSalesperson();
        purchaseOrder.fetchOrders();
    }

    BuyOrderView {
        id: listView
        anchors.fill: parent
        model: listModel
        stackView: root.myStack
    }

    RoundButton {
        onClicked: {
            buyOrderForm.open();
        }

        anchors {
            bottom: parent.bottom
            bottomMargin: 20
            right: parent.right
            rightMargin: 15
        }

        text: "+"
        font.pointSize: 14
        Material.background: Material.Blue
        Material.foreground: "white"
    }

    ListModel { id: listModel }

    BuyOrderForm {
        id: buyOrderForm
        width: ApplicationWindow.window.width
        height: ApplicationWindow.window.height
        onAccepted: {
            if (root.salesperson.salespersonType === Salesperson.Administrator)
            {
                purchaseOrder.placeOrder(buyOrder);
                return;
            }

            const creditAvailable = root.salesperson.creditAvailable;
            if (creditAvailable >= totalAmount)
                api.createBuyOrder(buyOrder);
            else {
                dialog.text = "No cuentas con suficiente credito";
                dialog.informativeText = "Credito disponible: "+creditAvailable;
                dialog.open();
            }
        }
    }

    RestService {
        PurchaseOrder {
            id: purchaseOrder
            path: `salespersons/${SalespersonSettings.salespersonId}/buy_orders`
            onOrdersFetched: resp => root.ordersReady(resp)
            onOrderPlaced: resp => root.orderPlacedHandler(resp)
            onErrorOccurred: err => root.errorOccurredHandler(err)
        }
    }

    SalespersonController {
        id: spController
        adminId: UserSettings.userId
        salespersonId: SalespersonSettings.salespersonId
    }

    MessageDialog {
        id: dialog
        onButtonClicked: close()
    }

    function ordersReady(resp) {
        const buyOrders = JSON.parse(resp).buy_orders;
        for (let item of buyOrders) {
            if(!item.thru_date)
            item.thru_date = item.thru_date || "";
            item.supplier = item.supplier || "";
        }
        listModel.append(buyOrders);
    }

    function orderPlacedHandler(resp) {
        const buyOrder = JSON.parse(resp).buy_order;
        listModel.append(buyOrder);
        ProductInventoryController.inventory.editFromList(buyOrder.order_details);

        if (root.salesperson.salespersonType === Salesperson.Administrator) {
            dialog.text = "Orden de compra creada con exito!";
            dialog.open();
            return;
        }

        const totalAmount = buyOrderForm.totalAmount;
        root.salesperson.creditAvailable -= totalAmount;
        root.salesperson.creditConsumed += totalAmount;

        dialog.text = "Orden de compra creada con exito!";
        dialog.informativeText = `En esta compra has consumido RD$${totalAmount} creditos. Ahora cuentas con RD$${salesperson.creditAvailable} creditos disponibles.`;
        dialog.open();

        buyOrderForm.close();
    }

    function errorOccurredHandler(err) {
        print("err: ", err);
        print("err: ", JSON.stringify(err));
        dialog.text = "Error al realizar Orden de Compra";
        dialog.informativeText = err;
        dialog.open();
    }
}
