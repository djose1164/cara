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

    function findById(productId: int): product {
        console.log("ProductManager.productId:", productId)
        const pred = product => product.id === productId;

        for (let i = 0; i < model.count; ++i)
            if (pred(model.get(i)))
                return model.get(i);

        return null;
    }
}
