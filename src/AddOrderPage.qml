import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/main"
import "qrc:/client"
import "qrc:/product"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
Page {
    id: root
    signal viewRequested

    header: AddOrderHeader {}
    onViewRequested: orderForm.updateClientField()

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        BusyIndicator {}

        ColumnLayout {
            Description {
                id: orderForm
                Layout.fillWidth: true
                Layout.fillHeight: true
                Component.onCompleted: productRequest.addSuggetionsToModel()
            }

            RowLayout {
                id: btns
                Layout. fillWidth: true
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: height/2
                spacing: 20

                Button {
                    id: okBtn
                    text: "Ok"
                    onClicked: okHandler()
                    Layout.fillWidth: true
                }
            }
        }

        MessageDialog {
            id: message
            buttons: MessageDialog.Ok
            onButtonClicked: close()
        }
    }

    ListModel { id: orderModel }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {stackLayout.currentIndex = 1; print("onSucceeded")}
    }

    Request {
        id: saver
        base: "http://127.0.0.1:5000/api"
        onReceived: stackLayout.currentIndex = 1;
        onFailed: {
            message.text = "No se pudo realizar la accion";
            message.open();
        }
        onSucceeded: {
            message.text = "Datos guardos";
            clearOrderForm();
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

    function okHandler() {
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

        saver.post("/orders/", fullOrder);
    }
}
