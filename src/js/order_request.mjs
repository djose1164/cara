import {Request} from "request.mjs";

const sender = WorkerScript.sendMessage;

class OrderRequest extends Request {
    constructor(model = null) {
        super(model);
    }

    saveOrder(order) {
        const endpoint = "/orders";
        print(JSON.stringify(order));
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, order, handle);
   }

   saveOrderDetails(details) {
        const endpoint = "/order_details";
        const handle = response => sender({"code": response.code});
        this.request("POST", endpoint, details, handle);
   }
                        
  findOrderById(clientId) {

        const handle = response => {
            sender({"code": response.code, "orders": response.orders});
            this.updateModel(response.orders);
        }
        this.request("GET", "/" + orderId, null, handle);
    }

    filterBillsBy(filter) {
        const handle = response => {
            sender({"code": response.code, "orders": response.orders});
            this.updateModel(response.orders);
        }
        this.request("POST", "/filter", filter, handle);
    }
}

WorkerScript.onMessage = obj => {
                print("---Orders---");
    const orderRequest = new OrderRequest();
    if (!obj.date)
        delete obj["date"];
    if (obj.client_id && obj.order_details)
        orderRequest.saveOrder(obj);
    if (obj.orderId)
        orderRequest.findOrderById(orderId);
    if (obj.filter)
        orderRequest.filterBillsBy(filter);
}
