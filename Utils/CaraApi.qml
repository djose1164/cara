import QtQuick
import Authentication
import data

Rest {
    id: root
    property string endpoint: ""
    base: Constants.base + endpoint
    accessToken: "Bearer " + TokenStore.access
}
