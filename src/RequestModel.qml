import QtQuick
import QtQuick.Controls

/**
* This component encapsulate a `ListModel` to use it to make requests.
*/
Item {
    property alias model: listModel
    property alias count: listModel.count
    required property string scriptSource
    property string code

    ListModel { id: listModel }

    WorkerScript {
        id: worker
        source: scriptSource
        onMessage: resp => {
            code = resp.code;
            //_messageToConsole("received: " + JSON.stringify(resp, null, 2));
            _messageToConsole("code: " + code);
        }
    }

    /**
    * Send a request with or without a model adjunct to it.
    * @param {Object} obj - Object to post to the request.
    * @param {bool=} withModel - To decide wether to adjunt the request or not.
    */
    function send(obj, withModel=true) {
        if (withModel)
            worker.sendMessage(Object.assign(obj, {"model": listModel}));
        else
            worker.sendMessage(obj);
        _messageToConsole("sent: " + JSON.stringify(obj, null, 2));
    }

    function find(func) {
        for (let i = 0; i < model.count; ++i)
            if (func(model.get(i)))
                return model.get(i);

        return null;
    }

    function _messageToConsole(msg) {
        print("[RequestModel]" + msg);
    }
}
