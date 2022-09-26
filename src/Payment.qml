import QtQuick
import "qrc:/js/payment_request.mjs" as PaymentRequest

Item {
    id: root
    required property int paymentId
    required property int newPaidAmount
    property bool successful: false
    property string message

    WorkerScript {
        id: worker
        source: "qrc:/js/payment_request.mjs"
        onMessage: response => {
                       if (response.code === "success")
                            successful = true;
                       else
                            root.message = response.message;
                       print(root.message);
                   }
    }

    function patchPayment() {
        if (paymentId != 0 && newPaidAmount != 0)
            worker.sendMessage({"type": "patchPayment", "value": {
                                       "id": paymentId, "paid_amount": newPaidAmount}
                               });
    }
}
