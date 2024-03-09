import QtQuick
import Authentication
import data

Rest {
    id: root
    property string endpoint: ""
    base: Constants.base + endpoint
    accessToken: tokenStore.access

    property var ts: TokenStore {
        id: tokenStore
        Component.onCompleted: restore()
    }
}
