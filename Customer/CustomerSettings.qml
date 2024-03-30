import QtQuick
import QtCore
import Customer

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
