import { Request } from "request.mjs"

const sender = WorkerScript.sendMessage;

//https://caraapi.herokuapp.com/api/clients
class ClientRequest extends Request {
    constructor(model = null, base = "https://caraapi.herokuapp.com/api/clients") {
        super(model, base);
    }

    findAll() {
        const handle = (response) => {
            const data = response.clients;
            let clients = [];
            for (let client of data)
                clients.push({"name": client.forename + " "
                    + (client.surname ? client.surname : ""), "identifier": client.id});
            sender({ "code": response.code, "clients": clients});
            this.updateModel(clients);
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

    createClient(data) {
        const handle = (response) => sender({"code": response.code});
        this.request("POST", "/", data, handle);
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
        default:
            throw new TypeError("None type matched.")
    }
}
