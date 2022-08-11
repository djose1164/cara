import { Request } from "qrc:/js/request.mjs"
// .pragma library

class ProductRequest extends Request {
    constructor(base = "https://caraapi.herokuapp.com/api/products") {
        super(base)
    }

    addProductToModel(model, id, name) {
        const handleResource = (resource) => {
            let fetched = resource.product || resource.products;
            model.clear();
            model.sync();
            model.append(fetched);
            model.sync();
            WorkerScript.sendMessage({ "code": "success" });
        }
        let endpoint = "/";
        if (id)
            endpoint += id;
        else if (name)
            endpoint += name;
        print("base: " + this.base);
        print("endpoint: " + endpoint);
        this.request("GET", endpoint, null, handleResource);
    }
}

WorkerScript.onMessage = (obj) => {
    const productRequest = new ProductRequest();
    if (obj.name && obj.model)
        productRequest.addProductToModel(obj.model, null, obj.name);
}
