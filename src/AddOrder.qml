import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import "qrc:/main"
import "qrc:/customers"
import "qrc:/product"
import "qrc:/users"
import com.cara 1.0

// TODO: Update implementation to use the new types.
Item {
    id: root
    property alias totalToPay: orderForm.total
    property alias stackLayout: stackLayout
    property alias orderForm: orderForm
    property alias request: request
    property alias orderModel: orderModel
    signal viewRequested

    onViewRequested: orderForm.updateClientField()

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        BusyIndicator {}

        Description {
            id: orderForm
            Component.onCompleted: productRequest.addSuggetionsToModel()
        }
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onButtonClicked: close()
    }

    ListModel { id: orderModel }

    ProductRequest {
        id: productRequest
        request.onSucceeded: stackLayout.currentIndex = 1;
    }

    Rest {
        id: request
        base: "http://127.0.0.1:5000/api"
        refreshToken: TheCurrentUser.refreshToken
        Component.onCompleted: refreshAccessToken()
        onReceived: stackLayout.currentIndex = 1
        onFailed: r => {
            message.text = "No se pudo realizar la accion";
            message.informativeText = "Error: " + r.message;
            message.open();
        }
        onSucceeded: r => {
            message.text = "Orden realizada con exito";
            message.informativeText = "Puedes observar el estado de la orden en"
                         + "Ordenes Recientes.";
            message.open();
            clearOrderForm();

            TheCurrentUser.orderPosted();

            if (TheCurrentUser.customerId === 0)
                TheCurrentUser.customerId = r.customer_id;
            print(JSON.stringify(r, null, 2));
        }
    }

    /**
    * Clear & prepare the form to be used on another order.
    */
    function clearOrderForm() {
        orderForm.resetEntries();
        orderForm.clear();
    }
}
