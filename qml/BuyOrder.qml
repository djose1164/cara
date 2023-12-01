import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Salesperson
import User
import "qrc:/utils"
import "qrc:/orders"
import "qrc:/singleton"

Page {
    id: root

    readonly property var myStack: StackView.view
    readonly property LocalUserStore userStore: LocalUserStore {}
    property var salesperson

    title: "Ordenes de Compra"
    header: TitleHeader {
        title: root.title
        onBackClicked: myStack.pop()
    }

    Component.onCompleted: {
        const spId = userStore.user.salespersonId;
        spController.getSalespersonById(spId);
        api.fetchBuyOrders();
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
        width: appStack.width
        height: appStack.height
        onAccepted: {
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

    BuyOrderAPI {
        id: api
        salespersonId: userStore.user.salespersonId
        onBuyOrdersFetched: orders => listModel.append(orders)
        onBuyOrderCreated: function(resp) {
            const totalAmount = buyOrderForm.totalAmount;
            root.salesperson.creditAvailable -= totalAmount;
            root.salesperson.creditConsumed += totalAmount;

            listModel.append(resp.buy_order);
            dialog.text = "Orden de compra creada con exito!";
            dialog.informativeText = `En esta compra has consumido RD$${totalAmount} creditos. Ahora cuentas con RD$${salesperson.creditAvailable} creditos disponibles.`;
            dialog.open();

            buyOrderForm.close();
        }
    }

    SalespersonController {
        id: spController
        onFetched: function(sp) {
            root.salesperson = sp;
        }
    }

    MessageDialog {
        id: dialog
        onButtonClicked: close()
    }
}
