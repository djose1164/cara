import QtQuick
import QtQuick.Controls

Item {
    property alias model: listModel
    required property string scriptSource

    ListModel { id: listModel }

    WorkerScript {
        id: worker
        source: scriptSource
    }

    function send(obj) {
        worker.sendMessage(Object.assign(obj, {"model": listModel}));
    }
}
