import QtQuick
import "qrc:/utils"

Item {
    id: root

    property alias request: request

    Request {
        id: request
        base: "http://127.0.0.1:5000/api/customers"
    }

    function create(client) {
        request.post("/", client, null);
    }
}
