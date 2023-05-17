import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/bills"
import "qrc:/customers"
import "qrc:/singleton"

Item {
    id: root

    property StackView stackView
    property TabBar bar

    width: 400
    height: 400

    Label {
        id: recentOrdersLbl
        anchors.top: parent.top
        text: "Compras realizadas recientemente "
        font.pointSize: 13

        IconImage {
            anchors.left: recentOrdersLbl.right
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/icons/arrow_forward.svg"
            sourceSize: "18x18"
        }

        // FIXME: Remove weak reference
        TapHandler { onTapped: bar.currentIndex = 2 }
    }

    StackLayout {
        id: recentordersSL
        width: parent.width
        height: parent.height * .2
        anchors.top: recentOrdersLbl.bottom
        anchors.topMargin: 20
        onCurrentIndexChanged: print("recentordersSL: "+currentIndex)
        currentIndex: isUserAndCustomer ? 0 : 2

        BusyIndicator {}

        ListView {
            id: recentOrders
            readonly property StackView stackView: root.stackView
            model: customerModel.model
            delegate: CustomerBill { orientation: ListView.Horizontal }
            orientation: ListView.Horizontal
        }

        Item {
            Label {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                text: TheCurrentUser.isGuest ?
                          "Necesitas registrarte para poder ver tus ordenes recientes."
                        : "Aun no haz realizado ninguna orden."
                font.pointSize: 14
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.WordWrap
            }
        }
    }

    CustomerModel {
        id: customerModel
        request.onSucceeded: recentordersSL.currentIndex = 1
    }

    function setCustomerOrders() {
        customerModel.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
}
