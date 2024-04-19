import QtQuick
import Cara.Authentication
import Cara.Data

Rest {
    id: root
    property string endpoint: ""
    base: Constants.base + endpoint
    accessToken: "Bearer " + TokenStore.access
}
