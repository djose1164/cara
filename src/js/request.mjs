const local_url = "http://127.0.0.1:5000/api";
const remote_url = "https://cara-api.azurewebsites.net/api";
class Request {
    constructor(model = null) {
        this.model = model;
        this.request_url = remote_url;
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
        const complete_url = this.request_url + endpoint;
        print(`request: ${complete_url}`);

        let xhr = new XMLHttpRequest();
        xhr.onload = function () {
            if (cb) {
                const str = xhr.responseText.toString().replace(/null/g, '""');
                print("[request]onload(): " + str);
                cb(JSON.parse(str));
            }
        }
        xhr.open(verb, complete_url);
        xhr.onerror = () => {
            print("no_internet");
            WorkerScript.sendMessage({ "code": "no_internet" });
        }
        xhr.onprogress = function(event) {
          alert(`Received ${event.loaded} of ${event.total}`);
        };
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.setRequestHeader('Accept', 'application/json');
        let data = obj ? JSON.stringify(obj) : '';
        xhr.send(data);
    }
}

export { Request };
