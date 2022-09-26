import { Request } from "request.mjs";

const sender = WorkerScript.sendMessage;

//http://127.0.0.1:5000
class PaymentRequest extends Request{
    constructor(model = null, base = "http://127.0.0.1:5000/api/payments") {
        super(model, base);
    }

    patchPayment(obj) {
        const handler = response => sender(response);
        this.request("PATCH", "/", obj, handler);
    }
}

WorkerScript.onMessage = obj => {
    let model;
    if ("model" in obj)
        model = obj.model;
    const paymentRequest = new PaymentRequest(model);
    switch (obj.type) {
    case "patchPayment":
        paymentRequest.patchPayment(obj.value);
    }
}
