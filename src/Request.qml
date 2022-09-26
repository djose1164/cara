import QtQuick

Item {
    property alias source: worker.source
    property alias sender: worker.sendMessage
    property string message
    property string code
    readonly property bool success: code === "success"

    WorkerScript {
        id: worker

        onMessage: (response) => {
                       code = response.code;
                       if ("message" in response)
                            message = response.message;
                   }
    }
}
