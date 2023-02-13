import QtQuick
import "qrc:/utils"

Item {
    id: root

    property alias request: request

    Request {
        id: request
        base: "https://cara-api.azurewebsites.net/api/clients"
    }

    function create(client) {
        request.post("/", client, null);
    }
}
