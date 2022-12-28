import QtQuick
import "qrc:/utils" as Utils

Utils.Request {
    source: "qrc:/js/client_request.mjs"

    function filter(msg) {
        sender(msg);
    }
}
