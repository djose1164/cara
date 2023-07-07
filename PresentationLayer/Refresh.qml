import QtQuick
import "qrc:/utils"

CaraApi {
    endpoint: "/users/refresh"

    signal accessTokenReceived(token: string)

    onSucceeded: r => accessTokenReceived(r.access_token)

    headers: {"Authorization": refreshToken}

    function refresh() {
        get("/");
    }
}
