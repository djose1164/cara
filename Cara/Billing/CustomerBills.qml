import QtQuick
import QtQuick.Controls

ListView {
    id: root

    property bool shouldRefresh
    property StackView stackView
    property alias billBusiness: billBusiness

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
