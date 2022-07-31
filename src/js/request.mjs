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
    xhr.onerror = () => WorkerScript.sendMessage({ "code": "no_internet"});
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.setRequestHeader('Accept', 'application/json');
    let data = obj ? JSON.stringify(obj) : '';
    xhr.send(data);
}

function loginHandler(obj) {
    let cb = (response) => {
        print("loginHandler: response: " + JSON.stringify(response));
        if (response.code === "success")
            worker({"code": response["code"], "currentUser": obj.username});
        else
             worker({"code": response["code"]});
    }
    request("POST", "users/login", obj, cb);
}

/*
  If none, the username or email, is passed the current user is assumed.
*/
function signupHandler(obj) {
    print("obj: " + JSON.stringify(obj))
    let cb = (response) => {
        print(`response.code: ${response.code}`);
        worker({"code": response["code"]});
    }

    request("POST", "users", obj, cb);
}

function userInfoHandler(user, model) {
    const cb = (response) => {
        print("userInfoHandler: response: " + JSON.stringify(response));
        model.append(response.user);
        model.sync(); // Important.
        worker({"code": response["code"]});
    }
    request("GET", "users/" + user, null, cb);

}

WorkerScript.onMessage = function (obj) {
    let temp = JSON.parse(JSON.stringify(obj));
    print("obj: " + JSON.stringify(obj));
    const type = "type" in temp ? temp.type : null;
    delete temp.type;
    let model = obj.model;
    switch (type)
    {
    case "login":
        loginHandler(temp);
        break;
    case "signup":
        signupHandler(temp);
        break;
    case "userInfo":
        if (obj.user === "")
            throw new Error("User cannot be empty. Either supply username or email.")
            userInfoHandler(obj.user, model);
        break;
    default:
        throw new TypeError("None of the valid types matched.")
    }

}
