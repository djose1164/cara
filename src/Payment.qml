import QtQuick
//import "qrc:/js/payment_request.mjs" as PaymentRequest

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
                       print("## " + root.message + "--" + response.code);
                   }
    }

    function patchPayment() {
        if (paymentId > 0 && newPaidAmount > -1)
            worker.sendMessage({"type": "patchPayment", "value": {
                                       "id": paymentId, "paid_amount": newPaidAmount}
                               });
        print("Patching payment ID#" + paymentId + "paid_amount: " + newPaidAmount);
    }
}
