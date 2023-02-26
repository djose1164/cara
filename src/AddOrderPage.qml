import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import "qrc:/main"
import "qrc:/customers"
import "qrc:/product"
import "qrc:/users"
import com.cara

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
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: height / 2
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

    ListModel {
        id: orderModel
    }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {
            stackLayout.currentIndex = 1
            print("onSucceeded")
        }
    }

    Rest {
        id: saver
        base: "http://127.0.0.1:5000/api"
        accessToken: TheCurrentUser.accessToken
        onReceived: stackLayout.currentIndex = 1
        onFailed: r => {
            message.text = "No se pudo realizar la accion";
            message.informativeText = "Error: " + r.message;
            message.open();
        }
        onSucceeded: {
            message.text = "Datos guardos";
            message.open();
            clearOrderForm();
        }
    }


    /**
    * Clear & prepare the form to be used on another order.
    */
    function clearOrderForm() {
        orderForm.resetEntries()
        orderForm.clear()
    }

    function okHandler() {
        stackLayout.currentIndex = 0
        const details = []
        for (let i = 0; i < orderModel.count; ++i)
            details.push(orderModel.get(i).entryValue)

        const fullOrder = {
            "customer_id": orderForm.clientID,
            "date": orderForm.date ? orderForm.date : new Date().toLocaleDateString(
                                         Qt.locale(), "dd/MM/yyyy"),
            "order_details": details,
            "payment": {
                "amount_to_pay": orderForm.total,
                "paid_amount": orderForm.amountPaid
            }
        }
        print("fullorder: " + JSON.stringify(fullOrder, null, 2))

        saver.post("/orders/", fullOrder)
    }
}
