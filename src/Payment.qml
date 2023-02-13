import QtQuick
import "qrc:/utils" as Utils

Item {
    id: root

    required property int paymentId
    required property int debt

    property string message
    property bool successful: request.code === "success"

    signal requested
    signal responseReceived

    Utils.Request {
        id: request
        base: "https://cara-api.azurewebsites.net/api/payments"
        onSent: root.requested();
        onReceived: responseReceived();

    }

    function patchPayment(amount) {
        print("Patching payment")
        if (paymentId < 1 && amount <= 0)
            return;

        request.patch("/", {"id": paymentId, "paid_amount": amount}, r => print("Payment: r: ", JSON.stringify(r, 4)));
        print("ID# " + paymentId + " paid_amount: " + amount);
    }

    function setAsUnpaid() {
        patchPayment(0);
    }

    function setAsPaid() {
        console.assert(debt !== 0, "Should not be zero");
        patchPayment(debt);
    }
}
