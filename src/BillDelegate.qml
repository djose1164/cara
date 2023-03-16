/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/customers"

ItemDelegate {
    id: root
    required property var person_info
    required property int id
    required property ListModel orders
    required property CustomerModel customerModel
    property int orientation : ListView.Vertical
    readonly property StackView stackView: ListView.view.stackView

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    onClicked: {
        stackView.push("qrc:/orders/UpdateOrderPage.qml", {
                           name: person_info.name,
                           customer_id: id,
                           orders
                       });
        // NOTE: Looks useless to update `UpdatePayment` component
        // onPaymentPatched
       /*const item = stackView.currentItem;
        item.paymentApplied.connect(function (payment) {
            print(`item.paymentApplied: ${JSON.stringify(payment, null, 2)}`);
            //customerModel.setCustomerPayment(payment);
        });*/
    }

    ColumnLayout {
        anchors.fill: parent
        Layout.leftMargin: 15
        Layout.alignment: Qt.AlignVCenter

        Label {
            Layout.topMargin: 15
            text: person_info.name
            font.pointSize: 16
        }

        Label {
            text: `Cliente ID # ${id}`
            font.pointSize: 12
            Layout.leftMargin: 15
        }
    }
}
