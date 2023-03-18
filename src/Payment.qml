import QtQuick
import "qrc:/utils" as Utils
import "qrc:/users"
import com.cara 1.0

QtObject {
    id: root

    required property var payment
    readonly property int debt: payment.amount_to_pay
    readonly property int totalDebt: debt - payment.paid_amount

    property string message
    property bool successful: request.code === "success"

    signal requested
    signal responseReceived
    signal paymentPatched(newPayment: var)

    Component.onCompleted: rest.refreshAccessToken()

    property Rest rest: Rest {
        id: request
        base: "https://cara-api.onrender.com/api/payments"
        accessToken: TheCurrentUser.accessToken
        onSent: root.requested()
        onReceived: responseReceived();
    }

    function patchPayment(amount) {
        print("Patching payment")
        if (payment.id < 1 && amount <= 0)
            return;

        request.succeeded.connect(function () {
            print("Payment: payment's amount paid updated.");
            payment.paid_amount = amount;
            if (debt - amount === 0)
                payment.status = 0; // Paid
            else if (amount > 0)
                payment.status = 2; // Credited
            else
                payment.status = 1; // Unpaid
            paymentPatched(payment)
        });
        request.patch("/", {"id": payment.id, "paid_amount": amount});
        print("ID# " + payment.id + " paid_amount: " + amount);
    }

    function setAsUnpaid() {
        patchPayment(0);
    }

    function setAsPaid() {
        console.assert(debt !== 0, "Should not be zero");
        patchPayment(debt);
    }
}
