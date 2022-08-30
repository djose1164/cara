import {Request} from "request.mjs"

const sender = WorkerScript.sendMessage;

class OrderRequest extends Request {
    constructor(model = null) {
        super(model);
        print("In OrderRequest: base: " + this.base);
    }

    saveOrder(order) {
        const endpoint = "/orders";
            print("order: " + JSON.stringify(order));
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, order, handle);
   }

   saverOrderDetails(details) {
        const endpoint = "/order_details";
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, details, handle);
   }
}

WorkerScript.onMessage = obj => {
    const orderRequest = new OrderRequest("http://127.0.0.1:5000/api");
    print("order_details: " + JSON.stringify(obj.order_details));
    if (!obj.date)
        delete obj["date"];
    if (obj.client_id && obj.order_details)
        orderRequest.saveOrder(obj);
}
