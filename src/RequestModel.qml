import QtQuick
import QtQuick.Controls

/**
* This component encapsulate a `ListModel` to use it to make requests.
*/
Item {
    property alias model: listModel
    property alias count: listModel.count
    property alias request: request
    property alias source: request.source

    property string code

    ListModel { id: listModel }

    Request { id: request }

    /**
    * Send a request with or without a model adjunct to it.
    * @param {Object} obj - Object to post to the request.
    * @param {bool=} withModel - To decide wether to adjunt the request or not.
    */
    function send(obj, withModel=true) {
        if (withModel)
            request.send(Object.assign(obj, {"model": listModel}));
        else
            request.send(obj);
    }

    function find(func) {
        for (let i = 0; i < model.count; ++i)
            if (func(model.get(i)))
                return model.get(i);

        return null;
    }

    function _messageToConsole(msg, from="[RequestModel]") {
        print(from + msg);
    }
}
