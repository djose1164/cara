import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import core 1.0
import models
import "qrc:/utils"
import "qrc:/orders"

ListView {
    id: root

    property bool shouldRefresh
    property StackView stackView
    property alias billBusiness: billBusiness
    property alias statusFilter: billProxyModel.statusFilter

    signal modelPopulated

    Component.onCompleted: billBusiness.fetchBillsByCustomerId(customerId)

    spacing: 20
    delegate: OrderState {
        required details
        required orderId
        required date
        required payment
        required status

        width: root.width
    }

    model: billProxyModel

    RefreshIndicator {
        id: refreshIcon
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: count === 0
    }

    Label {
        id: label
        visible: false
        anchors.centerIn: parent
        width: root.width*.9
        text: CurrentUser.isCustomer ? "No cuentas con ninguna compra."
                                     : "Este cliente no cuenta con ninguna orden."
        font.pointSize: 14
        wrapMode: Label.WordWrap
        horizontalAlignment: Label.AlignHCenter
    }

    BillModel {
        id: billModel
    }

    BillBusiness {
        id: billBusiness
        model: billModel
        onPopulated: modelPopulated();
        onOrdersNotFound: {
            print("billBusiness: onOrdersNotFound")
            busyIndicator.visible = false;
            label.visible = true;
        }
    }

    BillProxyModel {
        id: billProxyModel
        sourceModel: billModel
    }
}
