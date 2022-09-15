import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    width: parent.width
    height: 200
    readonly property int debt: payment.amount_to_pay
    Label {
        id: orderLbl
        text: "Orden #" + id + " --- Cliente ID #" + client_id
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
            visible: payment.status === 2
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
        anchors.centerIn: parent
        debt: debt
    }
}
