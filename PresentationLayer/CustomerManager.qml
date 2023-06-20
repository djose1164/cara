pragma Singleton
import QtQuick

QtObject {
    readonly property ListModel model: ListModel {}

    function modelIsEmpty() {
        return model.count === 0;
    }
}
