import QtQuick
import com.cara
import "qrc:/utils"

QtObject {
    id: root

    signal fetchedAll(resp: var)
    signal fetchedInventoriesByProductId(inventories: var)

    readonly property CaraApi api: CaraApi {
        endpoint: "/inventory"
        accessToken: CurrentUser.accessToken
    }

    function fetchAll() {
        api.get(`/?admin_id=${CurrentUser.user.userId}`);
        api.succeeded.connect(fetchedAll);
    }

    function fetchByProductId(productId: int) {
        api.get(`/?product_id=${productId}`);
        api.succeeded.connect(resp => fetchedInventoriesByProductId(resp.inventories));
    }
}
