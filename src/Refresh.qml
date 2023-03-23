import QtQuick
import "qrc:/utils"

CaraApi {
    endpoint: "/users/refresh"
    accessToken: null

    signal accessTokenReceived(token: string)

    onSucceeded: r => accessTokenReceived(r.access_token)

    function refresh() {
        get("/");
    }
}
