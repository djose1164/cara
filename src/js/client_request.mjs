import { Request } from "request.mjs"

const sender = WorkerScript.sendMessage;

class ClientRequest extends Request {
    constructor(model, base = "https://caraapi.herokuapp.com/api/clients") {
        super(base);
        this.model = model;
    }

    updateModel(data, shouldClear=false) {
            if (shouldClear) {
                this.model.clear();
                this.model.sync();
            }
            this.model.append(data);
            this.model.sync();
        }

    findAll() {
        const handle = (response) => {
            const data = response.clients;
            sender({"code": "success", "clients": data})
            this.updateModel(data)
        }
        this.request("GET", "/", null, handle);
        }

    findClientById(id) {
        const handle = (response) => sender({
            "code": "success", "client": response.client
        });
        this.request("GET", "/" + id, null, handle);
    }

    findClientByName(name) {
        const handle = (response) => sender({
            "code": "success", "clients": response.clients
        });
        this.request("GET", "/" + name, null, handle);
    }

    findOrders(client_id) {
        const handle = (response) => sender({
            "code": "success", "orders": response.client.orders
        });
        this.request("GET", "/" + client_id, null, handle);
    }
}

WorkerScript.onMessage = obj => {
    const clientRequest = new ClientRequest(obj.model);
    switch (obj.type) {
        case "findClientById":
            clientRequest.findClientById(obj.value);
            break;
        case "findClientByName":
            clientRequest.findClientByName(obj.value);
            break;
        case "findOrders":
            clientRequest.findOrders(obj.value);
            break;
        case "findAll":
            clientRequest.findAll();
            break;
        default:
            throw new TypeError("None type matched.")
    }
}
