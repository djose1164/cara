import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"

Page {
    id: root

    header: AddOrderHeader {}

    ColumnLayout {
        width: parent.width*.95
        height: parent.height*.95
        anchors.centerIn: parent

        AddOrder {
            id: addOrder
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Label {
            text: `El total asciende a RD${addOrder.totalToPay}`
        }

        Button {
            id: okBtn
            text: "Ok"
            onClicked: okHandler()
            Layout.fillWidth: true
        }
    }

    function okHandler() {
        addOrder.stackLayout.currentIndex = 0;
        const details = [];
        for (let i = 0; i < addOrder.orderModel.count; ++i)
            details.push(addOrder.orderModel.get(i).entryValue);

        const fullOrder = {
            customer_id: TheCurrentUser.customerId,
            date: addOrder.orderForm.date ? addOrder.orderForm.date : new Date().toLocaleDateString(
                                         Qt.locale(), "dd/MM/yyyy"),
            order_details: details,
            payment: {
                amount_to_pay: addOrder.totalToPay,
                paid_amount: 0
            }
        };
        print('fullOrder ', JSON.stringify(fullOrder, null, 2));
        addOrder.request.post("/orders/", fullOrder);
    }
}
