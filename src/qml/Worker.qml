import QtQuick

WorkerScript {
    source: "qrc:/js/database.mjs"

    signal resultReady(code: string)

    onMessage: (msgObject) => resultReady(msgObject.code)
}
