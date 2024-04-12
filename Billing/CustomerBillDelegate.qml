import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import User

Item {
    id: root

    property int orderId
    property date orderDate
    property var customer
    property var payment
    property var details
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
                visible: UserSettings.userTypeId !== UserEntity.Customer
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
}
