import QtQuick
import com.cara 1.0

Rest {
    base: "https://cara-rd.live/api/users/refresh"

    signal accessTokenReceived(token: string)

    onSucceeded: r => accessTokenReceived(r.access_token)

    function refresh() {
        get("/");
    }
}
