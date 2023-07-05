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
        accessToken: CurrentUser.accessToken
        onSucceeded: resp => customersFetched(resp.customers)

    }

    function fetchCustomers() {
        rest.get(`/?admin_id=${1}`);
    }
}
