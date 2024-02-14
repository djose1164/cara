pragma Singleton

import QtCore
import Salesperson

Settings {
    id: root

    property int salespersonId
    property int salespersonTypeId
    property string salespersonTypeName
    property int creditAvailable
    property int creditConsumed
    property int creditLimit

    category: "Salesperson"

    function entity(): salesperson {
        return {
            salespersonId,
            salespersonType: salespersonTypeId,
            creditLimit,
            creditConsumed,
            creditAvailable
        }
    }

    function clear() {
        root.salespersonId = 0;
        root.salespersonTypeId = 0;
        root.salespersonTypeName = "";
        root.creditAvailable = 0;
        root.creditConsumed = 0;
        root.creditLimit = 0;
    }
}
