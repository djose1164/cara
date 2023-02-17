import QtQuick

QtObject {
    id: root
    property string message
    property string code: ""
    required property string base

    signal sent
    signal received(var response)
    signal succeeded(var response)
    signal failed(var response)

    function request(verb, endpoint, obj, cb ) {
        const complete_url = base + endpoint;
        print(`complete_url: ${complete_url} verb: ${verb}`);

        let xhr = new XMLHttpRequest;
        xhr.open(verb, complete_url);

        xhr.onload = function () {
            //if (xhr.status !== 500)

            const resp = JSON.parse(xhr.responseText.toString());
            if ("message" in resp)
                root.message = resp.message;
            print(`root.code: ${root.code}`);
            print(`resp.code: ${resp.code}`);
            root.code = resp.code;

            root.received(resp);

            if (xhr.status === 200 || xhr.status === 201)
                root.succeeded(resp);
            else
                root.failed(resp);

            if (cb)
                cb(resp);
        }

        xhr.onerror = () => {
            root.code = "no_internet";
            if (cb)
                cb({ "code": "no_internet" });
        }

        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.setRequestHeader('Accept', 'application/json');

        let data = obj ? JSON.stringify(obj) : '';
        xhr.send(data);
        root.sent();
    }

    function post(endpoint, obj, cb) {
        request("POST", endpoint, obj, cb);
    }

    function get(endpoint, obj, cb) {
        request("GET", endpoint, obj, cb);
    }

    function put(endpoint, obj, cb) {
        request("PUT", endpoint, obj, cb);
    }

    function patch(endpoint, obj, cb) {
        request("PATCH", endpoint, obj, cb);
    }
}
