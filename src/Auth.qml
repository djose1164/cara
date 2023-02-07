import QtQuick
import QtCore
import "qrc:/utils"

Item {
    id: root
    property alias request: request
    property alias credentials: settings

    // Triggered if the login was succesful.
    signal authenticated
    signal invalid

    Request {
        id: request
        onMessageReceived: success ? authenticated() : invalid()
        function send(entry) { print("Auth") }
    }

    Settings {
        id: settings
        category: "Credentials"
    }

    function authenticate(entity) {
        /*let xhr = new XMLHttpRequest;
        xhr.open("POST", "https://cara-api.azurewebsites.net/api/users/login");
        xhr.onload = () => {
            let response = xhr.responseText.toString();
            print("-- " + response);
            response = JSON.parse(response);
            request.code = response.code;
            request.messageReceived(response);
        }
        xhr.onreadystatechange = () => print("readystate: " + xhr.readyState);
        xhr.send(JSON.stringify(entity));
        print("sent")
        request.messageSent();*/
        //request.send(entity);
        request.post("/users/login", entity);
    }
}
