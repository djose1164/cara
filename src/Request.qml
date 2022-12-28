import QtQuick
import QtQuick.Controls

Item {
    property alias source: worker.source
    readonly property var sender: worker.sendMessage
    property string message
    property string code
    readonly property bool success: code === "success"
    property ListModel model: null
    property BusyIndicator busyIndicator: null

    WorkerScript {
        id: worker

        onMessage: (response) => {
           code = response.code;
           if ("message" in response)
                message = response.message;
            print("###model.count:" + model.count)
       }
    }
}
