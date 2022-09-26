import { Request } from "request.mjs"

const sender = WorkerScript.sendMessage;

class LoginRequest extends Request {
    constructor(model = null, base = "https://caraapi.herokuapp.com/api/users") {
        super(model, base);
        this.isVerified = false;
    }

    get verify() {
        return this.isVerified;
    }

    set verify(verify) {
        this.isVerified = verify;
    }

    authenticate(obj) {
        let cb = (response) => {
            if (response.code === "success")
                sender({ "code": response["code"], "currentUser": obj.username });
            else
                sender({ "code": response["code"] });
        }
        this.request("POST", "/login", obj, cb);
    }

    /*
      If none, the username or email, is passed the current user is assumed.
    */
    signUp(obj) {
        let cb = (response) => sender(response["code"]);

        this.request("POST", "/", obj, cb);
    }

    userInfo(user) {
        const cb = (response) => {
            this.updateModel(response.user);
            sender({ "code": response["code"] });
        }
        this.request("GET", "/" + user, null, cb);
    }
}

WorkerScript.onMessage = (obj) => {
    let loginRequest = new LoginRequest(obj.model);
    if ("model" in obj)
        loginRequest.userInfo(obj.username);
    else if ("username" in obj && "password" in obj)
        loginRequest.authenticate(obj);
    else if ("email" in obj)
        loginRequest.signUp(obj);
}
