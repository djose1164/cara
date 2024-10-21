import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    required property var model

    signal clicked

    implicitWidth: pane.implicitWidth
    implicitHeight: pane.implicitHeight

    Pane {
        id: pane
        anchors.fill: parent
        anchors.margins: 10
        font.pointSize: 12
        Material.elevation: 6

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

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
            }

            Label {
                visible: model.comment.length > 0
                text: "<b>Último comentario:</b> " + model.comment
                elide: Label.ElideRight
                wrapMode: Label.Wrap
                Layout.fillWidth: true
                Layout.fillHeight: true
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
            return "pending.svg";
        case OrderQueue.Assigned:
            return "assigned.png";
        case OrderQueue.Completed:
            return "order-completed.svg"
        }
    }
}
