import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

BackPage {
    id: root

    title: "Mi Carrito"

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
                onPayClicked: CartBusiness.checkout()
                Layout.fillWidth: true
                total: CartBusiness.totalToPay
            }
        }
    }

    Connections {
        target: CartBusiness

        function onCheckoutSucceeded(resp) {
            const title = "¡Gracias por su compra!";
            const content = "Puedes ver el estado de tu orden en Ordenes."
            message.text = title;
            message.detailedText = content;
            message.open();
        }

        function onCheckoutFailed(resp) {
            message.text = `Error: ${resp.code}`;
            message.detailedText = resp.message;
            message.open();
        }
    }

    Message {
        id: message
    }
}
