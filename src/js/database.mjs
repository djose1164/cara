const worker = WorkerScript.sendMessage;

function request(verb, endpoint, obj, cb) {
    const BASE = "https://caraapi.herokuapp.com/api";
    print('request: ' + verb + ' ' + BASE + (endpoint ? '/' + endpoint : ''));
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        if (cb)
            cb(JSON.parse(xhr.responseText.toString()));
    }
    xhr.open(verb, BASE + (endpoint ? '/' + endpoint : ''));
    xhr.onerror = () => WorkerScript.sendMessage({ "code": "no_internet", "busy": false });
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Accept', 'application/json');
    let data = obj ? JSON.stringify(obj) : '';
    xhr.send(data);
}

function loginHandler(obj) {
    let cb = (response) => response["code"] === "success" ?
            worker({ "code": "success", "busy": false })
          : worker({ "code": "failed", "busy": false });

    request("POST", "users/login", obj, cb)
}

function signupHandler(obj) {
    print("obj: " + JSON.stringify(obj))
    let cb = (response) => {
        print(`response.code: ${response.code}`);
        worker({"code": response["code"], "busy": false});
    }

    request("POST", "users", obj, cb);
}

WorkerScript.onMessage = function (obj) {
    let temp = JSON.parse(JSON.stringify(obj));
    print("obj: " + JSON.stringify(obj));
    const type = "type" in temp ? temp.type : null;
    delete temp.type
    switch (type)
    {
    case "login":
        loginHandler(temp);
        break;
    case "signup":
        signupHandler(temp);
        break;
    default:
        throw new TypeError("None of the valid types matched.")
    }

}
