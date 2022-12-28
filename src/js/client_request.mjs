import { Request } from "request.mjs"

const sender = WorkerScript.sendMessage;

//https://caraapi.herokuapp.com/api/clients
//http://127.0.0.1:5000
class ClientRequest extends Request {
    constructor(model = null, base = "https://cara-api.onrender.com/api/clients/") {
        super(model, base);
    }

    findAll() {
        const handle = (response) => {
            const data = response.clients;
            sender({ "code": response.code, "clients": data});
            this.updateModel(data);
        }

        this.request("GET", "/", null, handle);
    }

    findClientById(id) {
        const handle = (response) => {
            sender({"code": response.code, "client": response.client});
            this.updateModel(response.client);
        }

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

    createClient(data) {
        const handle = (response) => sender({"code": response.code});
        this.request("POST", "/", data, handle);
    }

    filter(data) {
        print("Filtering from client_request");
        const handle = response => {
            const data = response.clients;
            print(JSON.stringify(data));
            print(JSON.stringify(data));
            sender({"code": "success", "orders": data});
            this.updateModel(data, true);
        }
        this.request("POST", "/filter", data, handle);
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
        case "createClient":
            clientRequest.createClient(obj.client)
            break;
        case "filter":
            print(JSON.stringify(obj));
            clientRequest.filter(obj.value);
            break;
        default:
            throw new TypeError("None type matched.")
    }
}
