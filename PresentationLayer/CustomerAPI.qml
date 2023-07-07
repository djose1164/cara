import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import com.cara 1.0

QtObject {
    id: root

    signal customersFetched(customers: var)

    default property var data

    property CaraApi rest: CaraApi{
        readonly property int adminId: CurrentUser.user.userId

        endpoint: "/customers"
        accessToken: TokenManager.accessToken

    }

    function fetchCustomers() {
        rest.get(`/info/?admin_id=${CurrentUser.user.userId}`);
        rest.succeeded.connect(resp => customersFetched(resp.customers));
    }
}
