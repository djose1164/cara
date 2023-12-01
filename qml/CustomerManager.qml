pragma Singleton
import QtQuick

QtObject {
    readonly property ListModel model: ListModel {}
    readonly property bool isEmpty: model.count === 0

    signal updateModel

    function addCustomer(customer) {
        model.append(customer);
    }

    function clearModel() {
        model.clear();
    }
}
