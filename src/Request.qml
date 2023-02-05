import QtQuick
import QtQuick.Controls

Item {
    property alias source: worker.source
    readonly property bool success: code === "success"
    property string message
    property string code

    signal messageSent
    signal messageReceived(variant response)

    WorkerScript {
        id: worker
        onMessage: (response) => {
                       code = response.code;

                       if ("message" in response)
                            message = response.message;

                       messageReceived(response);
                   }
    }

    function send(entry) {
        worker.sendMessage(entry);
        messageSent();
    }
}
