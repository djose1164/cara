import QtQuick
import "qrc:/utils"


CaraApi {
    id: api
    endpoint: "/products"
    onFailed: r => print(JSON.stringify(r, null, 2))

    function patchStock(inStock: int, success, fail): void {
        failed.connect(fail);
        patchProduct({"in_stock": inStock}, success);
    }

    function patchName(name: string): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.name = name;
        };

        patchProduct({"name": name}, success);
    }

    function patchSellPrice(sellPrice: int): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.sell_price = sellPrice;
        };

        patchProduct({"sell_price":sellPrice}, success);
    }

    function patchDescription(description: int): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.description = description;
        };

        patchProduct({"description": description}, success);
    }

    function patchProduct(toPatch: var, onSuccess: var): void {
        if (onSuccess)
            api.succeeded.connect(onSuccess);
        api.patch(`/${productId}`, toPatch);
    }
}
