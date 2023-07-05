import QtQuick
import com.cara
import "qrc:/utils"

QtObject {
    id: root

    signal fetchedAll(resp: var)

    readonly property CaraApi api: CaraApi {
        endpoint: "/inventory"
        accessToken: CurrentUser.accessToken
    }

    function fetchAll() {
        print("holalalla*-*-*-*-*-*-*-*-*-*-")
        api.get(`/?admin_id=${CurrentUser.user.userId}`);
        api.succeeded.connect(fetchedAll);
    }
}
