import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/main"
import "qrc:/client"

// TODO: Update implementation to use the new types.

StackLayout {
    id: stackLayout

    // The user has selected this view, that is, this component.
    signal viewRequested

    onViewRequested: orderForm.updateClientField()

    BusyIndicator {}

    ScrollView {
        id: scrollView
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        Page {
            id: root
            width: parent.width * .8

            anchors.horizontalCenter: parent.horizontalCenter
            ColumnLayout {
                id: columnLayout

                spacing: 20

                Description {
                    id: orderForm
                    Layout.fillWidth: true
                    Component.onCompleted: worker.sendMessage({"model": productModel})
                }

                RowLayout {
                    id: btns
                    Layout. fillWidth: true
                    spacing: 20

                    Button {
                        id: okBtn
                        text: "Ok"
                        onClicked: {
                            stackLayout.currentIndex = 0
                            const details = [];
                            for (let i = 0; i < orderModel.count; ++i)
                                details.push(orderModel.get(i).entryValue);

                            const fullOrder = {
                                "client_id": orderForm.clientID,
                                "date": orderForm.date ? orderForm.date
                                                       : new Date().toLocaleDateString(Qt.locale(), "dd/MM/yyyy"),
                                "order_details": details,
                                "payment": {
                                    "amount_to_pay": orderForm.total,
                                    "paid_amount": orderForm.amountPaid
                                }
                            };
                            print("fullorder: " + JSON.stringify(fullOrder, 2));

                            saver.sendMessage(fullOrder);
                        }
                        Layout.fillWidth: true
                    }
                }
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
                               if (response.code === "success") {
                                   message.text = "Datos guardos";
                                   clearOrderForm();
                               }
                               else
                                   message.text = "No se pudo realizar la accion";
                               message.open();
                           }
            }
        }
    }
    /*AddOrderHeader { id: headerToolBar }
    Component.onCompleted: app.header = headerToolBar*/

    /**
    * Clear & prepare the form to be used on another order.
    */
    function clearOrderForm() {
        orderForm.resetEntries();
        orderForm.clear();
    }
}
