import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import "qrc:/utils"
import "qrc:/orders"
import "qrc:/filters"

Page {
    id: root

    property StackView stackView: StackView.view
    property int customerId
    property int orderId
    property string name
    property date date
    property var payment
    property var status
    property var details

    title: CurrentUser.isAdmin ? "Cliente: "+name+" ID #"+customerId : "Estado de Orden"
    header: titleHeader

    TitleHeader {
        id: titleHeader
        btnLoader.sourceComponent: FilterButton {
            onClicked: paymentStatusFilter.open()
        }
        title: root.title
        onBackClicked: root.stackView.pop()
    }

    StackLayout {
        id: stackLayout
        anchors {
            fill: parent
            leftMargin: 10
            rightMargin: 5
        }
        currentIndex: CurrentUser.isAdmin ? 1 : 0

        OrderState {
            payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
        }

        CustomerBills {
            id: customerBills
        }
    }
}
