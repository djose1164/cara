pragma Singleton
import QtQuick

QtObject {
    id: root

    readonly property ListModel model: ListModel {}

    function addtoCart(product: var) {
        model.append(product);
    }

    function removeFromCart(index: int) {
        model.remove(index);
    }
}
