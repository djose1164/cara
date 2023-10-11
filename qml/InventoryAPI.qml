import QtQuick
import core
import "qrc:/utils"
import "qrc:/singleton"

QtObject {
    id: root

    signal fetchedAll(resp: var)
    signal fetchedInventoriesByProductId(inventories: var)
    signal fetchedByAdminIdAndProductId(inventory: var)

    readonly property CaraApi api: CaraApi {
        endpoint: "/inventory"
        accessToken: TokenManager.accessToken
    }

    function fetchAll() {
        api.get(`/?admin_id=${CurrentUser.user.userId}`);
        api.succeeded.connect(fetchedAll);
    }

    function fetchByProductId(productId: int) {
        api.get(`/?product_id=${productId}`);
        api.succeeded.connect(resp => fetchedInventoriesByProductId(resp.inventories));
    }

    function fetchByAdminIdAndProductId(adminId: int, productId: int) {
        api.get(`/?product_id=${productId}&admin_id=${adminId}`);
        api.succeeded.connect(resp => fetchedByAdminIdAndProductId(resp.inventory));
    }
}
