import { Request } from "qrc:/js/request.mjs"
// .pragma library

class ProductRequest extends Request {
    constructor(model = null, base = "https://caraapi.herokuapp.com/api/products") {
        super(model, base);
    }

    addProductToModel(id, name) {
        const handleResource = (resource) => {
            let fetched = resource.product || resource.products;
            this.updateModel(fetched);
            WorkerScript.sendMessage({ "code": "success" });
        }
        let endpoint = "/" + (id ? id : (name ? name : ""));
        this.request("GET", endpoint, null, handleResource);
    }


}

WorkerScript.onMessage = (obj) => {
    const productRequest = new ProductRequest(obj.model);
    if (obj.name && obj.model)
        productRequest.addProductToModel(null, obj.name);
    else if (obj.model)
        productRequest.addProductToModel(null, null);
}
