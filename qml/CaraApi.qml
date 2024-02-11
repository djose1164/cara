import QtQuick
import Authentication
import data

Rest {
    property string endpoint: ""
    base: Constants.base + endpoint
    accessToken: TokenManager.accessToken
}
