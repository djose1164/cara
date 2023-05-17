import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import "qrc:/customers"
import "qrc:/product"
import "qrc:/singleton"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
Item {
    id: root
    property alias totalToPay: orderForm.total
    property alias orderForm: orderForm
    property alias request: request
    property alias messageDialog: message

    Description {
        id: orderForm
        anchors.fill: parent
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onButtonClicked: close()
    }

    CaraApi {
        id: request
        endpoint: "/orders"
        accessToken: TheCurrentUser.accessToken
        onFailed: r => {
            message.text = "No se pudo realizar la accion";
            message.informativeText = "Error: " + r.message;
            message.open();
        }
        onSucceeded: r => {
            message.text = "Orden realizada con exito";
            message.informativeText = "¡Gracias por su compra!";
            message.detailedText = "Puedes observar el estado de la orden en Ordenes Recientes.";
            message.open();
            clearOrderForm();

            if (TheCurrentUser.isCostumer && TheCurrentUser.customerId === 0)
                TheCurrentUser.customerId = r.customer_id;
            print(JSON.stringify(r, null, 2));
        }
    }

    /**
    * Clear & prepare the form to be used on another order.
    */
    function clearOrderForm() {
        orderForm.productEntries.resetEntries();
        orderForm.clear();
    }

    function entryFormData(): var {
        return orderForm.productEntries.entryValues();
    }
}
