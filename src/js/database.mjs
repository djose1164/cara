function request(verb, endpoint, obj, cb) {
    print("Inside request function!");
    const BASE = "https://caraapi.herokuapp.com/api";
    print('request: ' + verb + ' ' + BASE + (endpoint ? '/' + endpoint : ''));
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        print('xhr: on ready state change: ' + xhr.readyState);
        if(xhr.readyState === XMLHttpRequest.DONE) {
            if(cb) {
                let res = JSON.parse(xhr.responseText.toString());
                cb(res);
            }
        }
    }
    xhr.open(verb, BASE + (endpoint ? '/' + endpoint : ''));
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Accept', 'application/json');
    var data = obj ? JSON.stringify(obj) : '';
    xhr.send(data);
}



WorkerScript.onMessage = function(obj) {
    const entry = {"username": obj.username, "password": obj.password}
    if (entry.username === "" || entry.password === "")
        throw new RangeError("Username or password out of range");

    console.log("In js: onMessage: busy: " + obj.busy);
    console.log("obj: " + JSON.stringify(obj));
    console.log("entry: " + JSON.stringify(entry));
    let cb = function(response) {
        console.log("response:" + JSON.stringify(response));
        if (response["code"] === "success")
            WorkerScript.sendMessage({"code": "success", "busy": false});
        else
            WorkerScript.sendMessage({"code": "failed", "busy": false});
    }

    request("POST", "users/login", entry, cb);

}
