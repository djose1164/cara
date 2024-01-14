import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import User
import core
import "qrc:/customers"

Item {
    id: root

    required property int userType
    property int orderId
    property int customerId
    property string name
    property date date
    property var bills
    property var details
    property var payment
    property var status

    property int orientation : ListView.Vertical

    readonly property int widthLV: ListView.view.width
    readonly property int heightLV: ListView.view.height
    readonly property bool isHorizontal: orientation === ListView.Horizontal

    width: orientation === ListView.Horizontal ? ListView.view.width*.65
                                               : ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .12

    StackLayout {
        anchors.fill: parent
        currentIndex: userType === UserEntity.Admin
                      || userType === UserEntity.Salesperson
                      ? 0 : 1

        AdminBillDelegate {
            id: adminDelegate

            name: root.name
            customerId: root.customerId
            onClicked: billStack.push("qrc:/bills/BillHomePage.qml", {customerId})
        }

        CustomerBillDelegate {
            id: customerDelegate

            onClicked: {
                billStack.push("qrc:/bills/BillDetailsPage.qml", {
                    details, date, payment, orderId, status
                });
            }

            orderId: root.orderId
            orderDate: root.date
        }
    }
}
