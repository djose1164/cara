import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/main"
import "qrc:/client"

StackLayout {
    id: stackLayout

    BusyIndicator {}

    Page {
        id: root
        Layout.fillWidth: true
        Layout.fillHeight: true
        header:  ToolBar {
            id: headerToolBar
            RowLayout {
                anchors.fill: parent

                ToolButton {
                    background: Image {
                        source: "qrc:/icons/left_arrow.svg"
                        antialiasing: false
                        sourceSize: iconSize
                    }
                    Layout.alignment: Qt.AlignLeft
                    onClicked: mainStackView.pop()
                }

                Label {
                    text: "Ordenes"
                    font.bold: true
                    Layout.alignment: Qt.AlignCenter
                }

                ToolButton {
                    background: Image {
                        source: "qrc:/icons/filter.svg"
                        antialiasing: false
                        sourceSize: iconSize
                    }
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        Label {
            id: lbl
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Crear nueva orden"
            font.bold: true
            font.pointSize: 18
        }

        Description {
            id: orderForm
            width: parent.width
            height: parent.height * .6
            anchors.top: lbl.bottom
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            Component.onCompleted: worker.sendMessage({"model": productModel})
        }

        RowLayout {
            id: btns
            width: parent.width * .9
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Button {
                id: okBtn
                text: "Ok"
                onClicked: {
                    stackLayout.currentIndex = 0
                    const details = [];
                    for (let i = 0; i < orderModel.count; ++i)
                        details.push(orderModel.get(i).entryValue);
                    const fullOrder = {"client_id": orderForm.clientID,
                        "date": orderForm.date ? orderForm.date : null,
                        "order_details": details,
                        "payment": {
                            "status": orderForm.payment,
                            "amount_to_pay": orderForm.total,
                            "amount_paid": orderForm.amountPaid
                        }
                    };
                    print("In okBtn: onClicked: " + JSON.stringify(orderModel))// This is available in all editors.
                    saver.sendMessage(fullOrder);
                }
                Layout.fillWidth: true
            }

            Button {
                text: "Nuevo cliente"
                Layout.fillWidth: true
                onClicked: clientForm.open()
            }
        }

        ClientForm {
            id: clientForm
        }

        MessageDialog {
            id: message
            buttons: MessageDialog.Ok
            onButtonClicked: close()
        }

        ListModel {
            id: productModel
        }

        ListModel {
            id: orderModel
        }

        WorkerScript {
            id: worker
            source: "qrc:/js/product_request.mjs"
            onMessage: stackLayout.currentIndex = 1
        }

        WorkerScript {
            id: saver
            source: "qrc:/js/order_request.mjs"
            onMessage: response => {
                           stackLayout.currentIndex = 1;
                           if (response.code === "success")
                                 message.text = "Datos guardos";
                           else
                                message.text = "No se pudo realizar la accion";
                           message.open();
                       }
        }

        Component.onCompleted: busyIndicator.open()
    }
}
