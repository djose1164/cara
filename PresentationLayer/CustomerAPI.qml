import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import com.cara 1.0

CaraApi {
    id: rest

    signal customersFetched(customers: var)

    endpoint: "/customers/info"
    accessToken: CurrentUser.accessToken
    onSucceeded: resp => customersFetched(resp.customers)
}
