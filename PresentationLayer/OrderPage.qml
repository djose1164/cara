import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/utils"
import "qrc:/orders"

Page {
    id: root

    property int customerId
    property int orderId
    property string name
    property date date
    property PaymentEntity payment
    property OrderModel details
    property BillModel bills

    readonly property bool isAdmin: CurrentUser.userType == CurrentUser.ADMIN

    header: TitleHeader {
        title: isAdmin ? "Cliente: "+name+" ID #"+customerId : "Estado de Orden"
        label.font.pointSize: 12
        //stackView: myStack
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: isAdmin ? 1 : 0

        OrderState {
            payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date

            //Component.onCompleted: paymentApplied.connect(root.paymentApplied)
        }

        CustomerBills {
            model: bills
        }
    }
}
