import QtQuick
import "qrc:/utils" as Utils
import "qrc:/users"
import com.cara 1.0

QtObject {
    id: root

    required property int paymentId
    required property int debt

    property string message
    property bool successful: request.code === "success"

    signal requested
    signal responseReceived
    signal paymentPatched()

    Component.onCompleted: rest.refreshAccessToken()

    property Rest rest: Rest {
        id: request
        base: "http://127.0.0.1:5000/api/payments"
        refreshToken: TheCurrentUser.refreshToken
        onSent: root.requested()
        onReceived: responseReceived();
        onSucceeded: a => paymentPatched()
    }

    function patchPayment(amount) {
        print("Patching payment")
        if (paymentId < 1 && amount <= 0)
            return;

        request.patch("/", {"id": paymentId, "paid_amount": amount});
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
