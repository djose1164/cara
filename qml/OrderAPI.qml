import QtQuick
import Cara.Utils

// TODO: Update implementation to use the new types.
CaraApi {
    id: request

    signal orderCreated(resp: var)

    endpoint: "/orders"

    onSucceeded: r => orderCreated(r)
}
