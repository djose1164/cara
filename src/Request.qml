import QtQuick
import QtQuick.Controls
import "qrc:/js/request.mjs" as Service

Item {
    readonly property bool success: code === "success"
    property string message
    property string code
    property string url: "https://cara-api.azurewebsites.net/api"

    signal messageSent
    signal messageReceived(variant response)

    onMessageReceived: resp => {
        if ("message" in resp)
            message = resp.code;
        code = resp.code;
        print("code " + code)
   }

    function request(verb, endpoint, obj) {
        let req = new Service.Request(url);
        req.request(verb, endpoint, obj, messageReceived);
        messageSent();
    }

    function post(endpoint, obj) {
        request("POST", endpoint, obj);
    }

    function update(endpoint, obj) {
        request("UPDATE", endpoint, obj);
    }

    function get(endpoint, obj) {
        request("GET", endpoint, obj);
    }

    function put(endpoint, obj) {
        request("PUT", endpoint, obj);
    }
}
