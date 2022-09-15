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
        ColumnLayout {
            id: columnLayout
            width: parent.width / 1.5
            height: parent.height/1.5
            anchors.centerIn: parent
            spacing: 20

            Component.onCompleted: print("Column: w: " + width + " h: " + height)
            Description {
                id: orderForm
                Layout.fillWidth: true
                Component.onCompleted: {
                    worker.sendMessage({"model": productModel})
                    print("w: " + width + " h: " + height)
                }
            }

            RowLayout {
                id: btns
                Layout. fillWidth: true
                spacing: 20

                Button {
                    id: okBtn
                    text: "Ok"
                    onClicked: {
                        print("## client_id" + orderForm.clientID)
                        stackLayout.currentIndex = 0
                        const details = [];
                        for (let i = 0; i < orderModel.count; ++i)
                            details.push(orderModel.get(i).entryValue);
                        const fullOrder = {
                            "client_id": orderForm.clientID,
                            "date": orderForm.date ? orderForm.date : null,
                            "order_details": details,
                            "payment": {
                                "status": orderForm.payment,
                                "amount_to_pay": orderForm.total,
                                "paid_amount": orderForm.amountPaid
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
        }
        ClientForm {
            id: clientForm
            onClosed: orderForm.updateClientField()
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
    }
    /*AddOrderHeader { id: headerToolBar }
    Component.onCompleted: app.header = headerToolBar*/
}
