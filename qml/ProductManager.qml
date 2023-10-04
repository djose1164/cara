pragma Singleton
import QtQuick

QtObject {
    readonly property ListModel model: ListModel {}

    signal updateModel()

    function modelIsEmpty() {
        return model.count === 0;
    }

    function modelAppend(obj) {
        model.append(obj);
    }

    function modelClear() {
        model.clear();
    }
}
