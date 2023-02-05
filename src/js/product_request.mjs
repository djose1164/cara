import { Request } from "qrc:/js/request.mjs";

const sender = WorkerScript.sendMessage;

class ProductRequest extends Request {
    constructor(model = null) {
        super(model);
        this.request_url += "/products";
    }

    addSuggetionsToModel(text) {
        const handleResource = (resource) => {
            let fetched = resource.product || resource.products;
            this.updateModel(fetched);
                print(fetched);
            sender(resource);
        }
        let endpoint = "/" + text;
        this.request("GET", endpoint, null, handleResource);
        print("Adding suggetions from worker");
    }

    addNewProduct(product) {
        const handler = resp => sender(resp);
        this.request("POST", "/", product, handler);
    }

    findProductByIdentifier(identifier) {
        this.request("GET", endpoint, null, response => sender(response));
    }
}

WorkerScript.onMessage = (obj) => {
    const productRequest = new ProductRequest(obj.model || null);
    if (obj.type === "addSuggetionsToModel" && obj.model)
        productRequest.addSuggetionsToModel(obj.identifier);
    else if (obj.type === "addNewProduct")
        productRequest.addNewProduct(obj.identifier);
    else if (obj.model)
        productRequest.addSuggetionsToModel("");
}
