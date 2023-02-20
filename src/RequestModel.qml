import QtQuick
import QtQuick.Controls

// TODO: Rethink about the design of this. Looks useless?

/**
* This component encapsulate a `ListModel` to use it to make requests.
*/
Item {
    property alias model: listModel
    property alias count: listModel.count
    property alias request: request

    ListModel { id: listModel }

    Request { id: request; base: "http://127.0.0.1:5000/api" }

    /**
    * Send a request with or without a model adjunct to it.
    * @param {Object} obj - Object to post to the request.
    * @param {bool=} withModel - To decide wether to adjunt the request or not.
    */
    function send(obj, withModel=true) {
        if (withModel)
            request.get("/", obj, response => model.append(response));
        else
            request.get("/", obj, null);
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
