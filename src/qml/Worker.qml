import QtQuick

WorkerScript {
    source: "qrc:/js/request.mjs"

    signal resultReady(code: string)

    onMessage: (obj) => resultReady(obj.code)
}
