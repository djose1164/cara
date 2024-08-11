import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    required property var model

    signal clicked

    Pane {
        anchors.fill: parent
        anchors.margins: 10
        Material.elevation: 6

        ColumnLayout {
            anchors.fill: parent

            GridLayout {
                columns: 2
                rows: 5
                columnSpacing: root.width * .1
                Layout.fillWidth: true

                ColumnLayout {
                    Layout.rowSpan: 5
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Image {
                        source: root._mapQueueStatusToIconUrl(model.queueStatusId)
                        sourceSize: Qt.size(80, 80)
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {
                        text: model.queueStatusString
                        font.pointSize: 14
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                Label {
                    text: "Order ID #" + model.orderId
                    font.pointSize: 14
                    font.bold: true
                }

                Label {
                    text: "<b>Estado de pago: </b>" + model.payment.statusString
                }

                Label {
                    text: "<b>Cantidad de productos: </b>" + model.productQuantity
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: "<b>Total: </b>RD$" + model.totalToPay
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: "<b>Comentario:</b> " + model.comment
                    wrapMode: Label.Wrap
                    Layout.fillWidth: true
                }
            }

            ToolSeparator {
                orientation: Qt.Horizontal
                Layout.fillWidth: true
            }

            ColumnLayout {

                Label {
                    text: "Order ID #" + model.orderId
                }
            }
        }

        TapHandler {
            onTapped: root.clicked()
        }
    }

    function _mapQueueStatusToIconUrl(statusId: int): url {
        switch (statusId) {
        case OrderQueue.Cancelled:
            return "cancelled-order.png";
        case OrderQueue.Unassigned:
            return "pending.png";
        case OrderQueue.Assigned:
            return "assigned.png";
        }
    }
}
