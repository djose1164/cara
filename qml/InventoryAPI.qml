import QtQuick
import User
import Utils

QtObject {
    id: root

    property int salespersonId

    signal fetchedAll(resp: var)
    signal fetchedInventoriesByProductId(inventories: var)
    signal fetchedByAdminIdAndProductId(inventory: var)

    readonly property CaraApi api: CaraApi {
        endpoint: "/inventory"
    }

    function fetchAll() {
        api.get(`/?salesperson_id=${salespersonId}`);
        api.succeeded.connect(fetchedAll);
    }

    function fetchByProductId(productId: int) {
        api.get(`/?product_id=${productId}`);
        api.succeeded.connect(resp => fetchedInventoriesByProductId(resp.inventories));
    }

    function fetchByAdminIdAndProductId(salespersonId: int, productId: int) {
        api.get(`/?product_id=${productId}&salesperson_id=${salespersonId}`);
        api.succeeded.connect(resp => fetchedByAdminIdAndProductId(resp.inventory));
    }
}
