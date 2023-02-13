import QtQuick
import "qrc:/utils"

Item {
    id: root
    property alias request: request

    // Triggered if the login was succesful.
    signal authenticated
    signal invalid

    Request {
        id: request
        base: "https://cara-api.azurewebsites.net/api/users/login"
        onSucceeded: authenticated()
        onFailed: invalid()
    }

    function authenticate(entity) {
        request.post("/", entity, null);
    }
}
