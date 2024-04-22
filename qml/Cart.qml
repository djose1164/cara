import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara
import Cara.Customer
import Cara.Shopping
import Cara.User
import Cara.Utils

Page {
    id: root

    readonly property customer buyer: ({customerId: UserSettings.customerId})

    title: "Mi Carrito"
    header: TitleHeader {
        title: root.title
        backBtn.visible: false
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: cartContent.count ? 1 : 0

        EmptyCart {}

        ColumnLayout { // TODO: Think about an abstracter concept for this.
            CartContent {
                id: cartContent
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Checkout {
                onPayClicked: CartController.checkout(root.buyer)
                Layout.fillWidth: true
                total: CartController.totalToPay
            }
        }
    }

    Connections {
        target: CartController

        function onCheckoutSucceeded(resp) {
            const title = "¡Gracias por su compra!";
            const content = "Puedes ver el estado de tu orden en Ordenes."
            message.text = title;
            message.informativeText = content;
            message.open();
        }

        function onCheckoutFailed(resp) {
            message.text = `Error: ${resp.code}`;
            message.informativeText = resp.message;
            message.open();
        }

        function onErrorOccurred(err: string) {
            message.text = "Error " + err;
            message.open();
        }
    }

    Message {
        id: message
    }

    RestService {
        CustomerResource {
            id: customerResource
            path: "customers"
        }
    }
}
