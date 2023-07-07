/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/users"
import "qrc:/customers"

Item {
    id: root
    property int orderId
    property int customerId
    property string name
    property date date
    property BillModel bills
    property OrderModel details
    property var payment
    property var status

    property int orientation : ListView.Vertical

    readonly property StackView stackView: ListView.view.stackView
    readonly property int widthLV: ListView.view.width
    readonly property int heightLV: ListView.view.height
    readonly property bool isHorizontal: orientation === ListView.Horizontal

    signal updateCustomer(customerId: int)

    width: orientation === ListView.Horizontal ? ListView.view.width*.65
                                               : ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    StackLayout {
        anchors.fill: parent
        currentIndex: CurrentUser.isAdmin ? 0 : 1

        AdminBillDelegate {
            id: adminDelegate

            onClicked: {
                stackView.push("qrc:/bills/OrderPage.qml", {
                    name, customerId
                });
            }

            name: root.name
            customerId: root.customerId
        }

        CustomerBillDelegate {
            id: customerDelegate

            onClicked: {
                stackView.push("qrc:/bills/OrderPage.qml", {
                    details, date, payment, orderId, status
                });
            }

            orderId: root.orderId
            date: root.date
        }
    }
}
