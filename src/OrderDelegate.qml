import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"

ColumnLayout {
    id: root
    width: parent.width
    height: parent.height * .3
    required property var payment
    required property var order_details
    required property string date
    required property string id

    signal dataChanged

    // BUG: For some reason this does not work. It works on 6.4.
    // Component.onCompleted: updatePayment.applied.connect(root.dataChanged);

    Label {
        id: orderLbl
        text: "Orden #" + id
        font.pointSize: 14
    }

    Label {
        id: dateBanner
        text: "Fecha"
        font.pointSize: 12
    }

    Label { text: date }

    Label {
        text: "Estado de de la orden: "
        font.pointSize: 12
    }

    GridLayout {
        Layout.fillWidth: true
        columns: 2

        Label {
            Layout.columnSpan: 2
            text: "Estado"
        }

        Label { text: statusToString(payment.status) }

        Button {
            text: "Actualizar"
            onClicked: updatePayment.open()
        }
    }

    Label {
        text: "Detalles"
        font.pointSize: 12
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 10

        Label { text: "Producto" }

        Label { text: "Cantidad" }
    }


    GridLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.topMargin: 20
        rowSpacing: 1
        columnSpacing: 20
        columns: 2

        Repeater {
            model: order_details

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Label { text: product.name }

                Label { text: quantity }
            }
        }
    }

    UpdatePayment {
        id: updatePayment
        debt: parseInt(payment.amount_to_pay) - parseInt(payment.paid_amount)
        paid: payment.paid_amount
        paymentID: payment.id
        onApplied: {
            print("OrderDelegate: UpdatePayment: onApplied");
            root.dataChanged();
        }
    }
}
