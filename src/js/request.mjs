export const sender = WorkerScript.sendMessage;

class Request {
    constructor(base = "https://caraapi.herokuapp.com/api") {
        this.base = base;
    }

    request(verb, endpoint, obj, cb) {
        const request = this.base + endpoint;
        print('request: ' + request);
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

    loginHandler(obj) {
        let cb = (response) => {
            print("loginHandler: response: " + JSON.stringify(response));
            if (response.code === "success")
                sender({ "code": response["code"], "currentUser": obj.username });
            else
                sender({ "code": response["code"] });
        }
        this.request("POST", "/users/login", obj, cb);
    }

    /*
      If none, the username or email, is passed the current user is assumed.
    */
    signupHandler(obj) {
        print("obj: " + JSON.stringify(obj))
        let cb = (response) => sender({ "code": response["code"] });

        this.request("POST", "/users", obj, cb);
    }

    userInfoHandler(user, model) {
        const cb = (response) => {
            print("userInfoHandler: response: " + JSON.stringify(response));
            model.append(response.user);
            model.sync(); // Important.
            sender({ "code": response["code"] });
        }
        this.request("GET", "/users/" + user, null, cb);
    }
}

export { Request };

WorkerScript.onMessage = function (obj) {
    const request = new Request()
    let temp = JSON.parse(JSON.stringify(obj));
    const type = "type" in temp ? temp.type : null;
    delete temp.type;
    let model = obj.model;
    switch (type) {
        case "login":
            request.loginHandler(temp);
            break;
        case "signup":
            request.signupHandler(temp);
            break;
        case "userInfo":
            if (obj.user === "")
                throw new Error("User cannot be empty. Either supply username or email.")
            request.userInfoHandler(obj.user, model);
            break;
        default:
            throw new TypeError("None of the valid types matched.")
    }
}
