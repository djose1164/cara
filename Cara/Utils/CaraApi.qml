import QtQuick
import Cara.Authentication
import Cara.Data

Item {
    id: root
    property string endpoint: ""

    signal succeeded(data: var)

    Rest {
        id: rest
        base: Constants.base + endpoint
        accessToken: "Bearer " + TokenManager.accessToken
        onSucceeded: data => root.succeeded(JSON.parse(data))
    }

    function get(endpoint: string) {
        rest.get(endpoint);
    }

    function post(endpoint: string, obj) {
        rest.post(endpoint, obj);
    }
}
