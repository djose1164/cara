import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/utils"
import "qrc:/orders"
import "qrc:/singleton"

HeaderPage {
    id: root
    readonly property StackView myStack: StackView.view

    Component.onCompleted: api.fetchBuyOrders();

    title: "Ordenes de Compra"

    BuyOrderView {
        id: listView
        anchors.fill: parent
        model: listModel
        stackView: root.stackView
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
        width: ViewManager.mainView.width
        height: ViewManager.mainView.height
        onAccepted: api.createBuyOrder(buyOrder);
    }

    BuyOrderAPI {
        id: api
        onBuyOrdersFetched: orders => listModel.append(orders)
        onBuyOrderCreated: buyOrder => {
            listModel.append(buyOrder);
            buyOrderForm.close();
            dialog.open();
        }
    }

    MessageDialog {
        id: dialog
        text: "Orden de compra creada con exito!"
        onAccepted: close()
    }
}
