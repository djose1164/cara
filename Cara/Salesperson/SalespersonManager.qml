pragma Singleton

import QtQuick

QtObject {
    readonly property ListModel model: ListModel {}
    readonly property bool isEmpty: model.count === 0

    function add(salesperson): void {
        model.append(salesperson);
    }
}
