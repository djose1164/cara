import QtQuick
import "qrc:/utils" as Utils

Item {
    id: root

    required property int paymentId
    required property int newPaidAmount

    property string message
    property bool successful: false

    property alias request: request

    Utils.Request {
        id: request
        source: "qrc:/js/payment_request.mjs"
    }

    function patchPayment() {
        if (paymentId > 0 && newPaidAmount > -1)
            request.send({"type": "patchPayment", "value": {
                                       "id": paymentId, "paid_amount": newPaidAmount}
                               });
        print("Patching payment ID# " + paymentId + "paid_amount: " + newPaidAmount);
    }
}
