class Request {
    constructor(model = null, base = "https://caraapi.herokuapp.com/api") {
        this.base = base;
        this.model = model;
    }

    updateModel(data, shouldClear = false) {
        if (!this.model)
            return;

        if (shouldClear) {
            this.model.clear();
            this.model.sync();
        }
        this.model.append(data);
        this.model.sync();
    }

    request(verb, endpoint, obj, cb) {
        const request = this.base + endpoint;
        let xhr = new XMLHttpRequest();
        xhr.onload = function () {
            if (cb)
                cb(JSON.parse(xhr.responseText.toString()));
        }
        xhr.open(verb, request);
        xhr.onerror = () => WorkerScript.sendMessage({ "code": "no_internet" });
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.setRequestHeader('Accept', 'application/json');
        let data = obj ? JSON.stringify(obj) : '';
        xhr.send(data);
    }
}

export { Request };
