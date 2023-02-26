import QtQuick
import com.cara

Rest {
    base: "http://127.0.0.1:5000/api/users/refresh"

    signal accessTokenReceived(token: string)

    onSucceeded: r => accessTokenReceived(r.access_token)

    function refresh() {
        get("/");
    }
}
