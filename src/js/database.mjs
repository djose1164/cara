const response = {
  1: "no_internet",
  2: "failed"
};

function request(verb, endpoint, obj, cb) {
    const BASE = "https://caraapi.herokuapp.com/api";
    print('request: ' + verb + ' ' + BASE + (endpoint ? '/' + endpoint : ''));
    var xhr = new XMLHttpRequest();
    xhr.onload = function() {
        print('xhr: on ready state change: ' + xhr.readyState);
        if(cb)
            cb(JSON.parse(xhr.responseText.toString()));
    }
    xhr.open(verb, BASE + (endpoint ? '/' + endpoint : ''));
    xhr.onerror = () => WorkerScript.sendMessage({"code": "no_internet", "busy": false});
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Accept', 'application/json');
    let data = obj ? JSON.stringify(obj) : '';
    xhr.send(data);
}

WorkerScript.onMessage = function(obj) {
    const worker = WorkerScript.sendMessage;
    const entry = {"username": obj.username, "password": obj.password}
    if (entry.username === "" || entry.password === "")
        throw new RangeError("Username or password out of range");
    const result = {response}
    let cb = (response) => response["code"] === "success" ?
            worker({"code": "success", "busy": false})
          : worker({"code": "failed", "busy": false});

    request("POST", "users/login", entry, cb)
}
