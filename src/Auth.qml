import QtQuick
import "qrc:/utils"

Item {
    id: root
    property alias request: request

    // Triggered if the login was succesful.
    signal authenticated(response: var)
    signal invalid

    Component.onCompleted: {
        request.succeeded.connect(authenticated);
        request.failed.connect(invalid);
    }

    Request {
        id: request
        base: "http://127.0.0.1:5000/api/users/login"
        //onSucceeded: authenticated()
        //onFailed: invalid()
    }

    function authenticate(entity) {
        request.post("/", entity, null);
    }
}
