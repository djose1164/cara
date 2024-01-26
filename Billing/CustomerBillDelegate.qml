import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int orderId
    property date orderDate
    property var customer
    property var details
    property var payment
    property var status
    readonly property string delegateUrl: "qrc:/orders/OrderState.qml"

    signal clicked

    Frame {
        width: root.width - 10
        height: root.height
        anchors.centerIn: parent
        Material.elevation: 8

        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Layout.fillWidth: true
                Label {
                    text: `Orden #${root.orderId}`
                    font.pointSize: 15
                    Layout.fillWidth: true
                }
                Label {
                    leftPadding: 14
                    Layout.alignment: Qt.AlignTrailing
                    text: `**Fecha:** ${root.orderDate.toLocaleDateString(Qt.locale(), Locale.ShortFormat)}`
                    textFormat: Label.MarkdownText
                    wrapMode: Label.Wrap
                }
            }

            Label {
                text: `
                    <strong>Cliente:</strong> ${customer.forename} ${customer.surname}
                `
                leftPadding: 14
            }

            Label {
                text: `<strong>Estado de pago: </strong>${payment.statusString}`
                leftPadding: 14
            }
        }

        TapHandler {
            onTapped: root.clicked()
        }
    }

    QtObject {
        id: internal

        function statusString(code: int) : string {
            const map = {
                1: "Pago",
                2: "Credito",
                3: "Abonado"
            }

            return map[code];
        }
    }
}
