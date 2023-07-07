import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/utils"
import "qrc:/orders"

HeaderPage {
    id: root

    property int customerId
    property int orderId
    property string name
    property date date
    property PaymentEntity payment
    property OrderStatus status
    property OrderModel details

    title: CurrentUser.isAdmin ? "Cliente: "+name+" ID #"+customerId : "Estado de Orden"

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: CurrentUser.isAdmin ? 1 : 0

        OrderState {
            payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
        }

        CustomerBills {}
    }
}
