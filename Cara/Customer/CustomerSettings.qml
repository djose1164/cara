import QtQuick
import QtCore
import Cara.Customer

Settings {
    id: root

    property int customerId

    category: "Customer"

    function entity(): customer {
        return {
            customerId
        }
    }
}
