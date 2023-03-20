import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"

Page {
    id: root
    readonly property var fullOrder: {
        "customer_id": TheCurrentUser.customerId,
        "date": addOrder.orderForm.date ? addOrder.orderForm.date
                                        : new Date().toLocaleDateString(
                                              Qt.locale(), "dd/MM/yyyy"),
        "order_details": addOrder.entryFormData(),
        "payment": {
            amount_to_pay: addOrder.totalToPay,
            paid_amount: 0
        }
    }

    header: AddOrderHeader {}

    ColumnLayout {
        anchors.fill: parent

        AddOrder {
            id: addOrder
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: root.height/2
            Layout.topMargin: 10
            Layout.alignment: Qt.AlignTop
        }

        Label {
            text: `El total asciende a RD$${addOrder.totalToPay}`
            font.pointSize: 14
            Layout.alignment: Qt.AlignCenter
        }

        Button {
            id: okBtn
            text: "Reservar"
            onClicked: {
                if (TheCurrentUser.isGuest) {
                    addOrder.messageDialog.text = "No has iniciado sesion."
                    addOrder.messageDialog.detailedText = "Necesitas registrarte para realizar reservaciones.";
                    addOrder.messageDialog.open();
                }
                else
                    addOrder.request.post("/orders/", fullOrder)
            }
            Layout.alignment: Qt.AlignCenter
        }
    }
}
