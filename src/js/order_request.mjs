import {Request} from "request.mjs";

const sender = WorkerScript.sendMessage;

// http://127.0.0.1:5000
// https://caraapi.herokuapp.com
class OrderRequest extends Request {
    constructor(model = null, base = "https://caraapi.herokuapp.com/api") {
        super(model, base);
    }

    saveOrder(order) {
        const endpoint = "/orders";
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, order, handle);
   }

   saverOrderDetails(details) {
        const endpoint = "/order_details";
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, details, handle);
   }
                        
  findByOrderId(clientId) {
        const handle = response => {
            sender({"code": response.code, "orders": response.orders});
            this.updateModel(response.orders);
        }
        this.request("GET", "/" + orderId, null, handle);
    }
}

WorkerScript.onMessage = obj => {
    const orderRequest = new OrderRequest();
    if (!obj.date)
        delete obj["date"];
    if (obj.client_id && obj.order_details)
        orderRequest.saveOrder(obj);
    if (obj.orderId)
        orderRequest.findByOrderId(orderId);
}
