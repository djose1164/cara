import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import "qrc:/customers"
import "qrc:/product"
import "qrc:/singleton"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
CaraApi {
    id: request

    signal orderCreated(resp: var)

    endpoint: "/orders"
    accessToken: TheCurrentUser.accessToken

    onSucceeded: r => orderCreated(r)
}
