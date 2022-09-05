import QtQuick

Item {
    ListModel { id: clientList }

    WorkerScript {
        id: workerScript
        source: "qrc:/js/client_request.mjs"
    }
}
