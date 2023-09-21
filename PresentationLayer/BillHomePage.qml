import QtQuick
import QtQuick.Controls
import com.cara 1.0
import "qrc:/utils"
import "qrc:/filters"
import "qrc:/singleton"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Todas las cuentas"
    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: FilterButton {
            onClicked: orderStatusFilter.open()
        }

        onBackClicked: stackView.pop()
    }
    Component.onCompleted: billBusiness.fetchBillsByAdminId(CurrentUser.user.userId)

    OrderStatusFilter {
        id: orderStatusFilter
        width: root.width*.8
        height: root.height/2
        onApplyFilter: statusId => orderStatusPM.statusFilter = statusId
    }

    ListView {
        id: listView
        anchors.fill: parent
        delegate: billDelegate
        model: orderStatusPM

        BusyIndicator {
            anchors.centerIn: parent
            visible: listView.count === 0
        }
    }

    Component {
        id: billDelegate
        CustomerBillDelegate {
            id: customerBillDelegate
            required property var model
             property var details: model.details
             property var payment: model.payment
             property var status: model.status

            width: root.width
            height: 100

            onClicked: {
                stackView.push("qrc:/bills/BillDetailsPage.qml", {
                    details, date, payment, orderId, status
                });
            }

            orderId: model.orderId
            date: model.date
        }
    }

    OrderStatusProxyModel {
        id: orderStatusPM
        sourceModel: BillManager.model
    }

    BillBusiness {
        id: billBusiness
        model: BillManager.model
    }
}
