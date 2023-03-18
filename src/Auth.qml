import QtQuick
import "qrc:/utils"
import com.cara 1.0

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

    Rest {
        id: request
        base: "https://cara-api.onrender.com/api/users/login"

    }

    function authenticate(entity) {
        request.post("/", entity);
    }
}
