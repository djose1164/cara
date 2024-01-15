import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Customer
import Product
import "qrc:/singleton"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
CaraApi {
    id: request

    signal orderCreated(resp: var)

    endpoint: "/orders"

    onSucceeded: r => orderCreated(r)
}
