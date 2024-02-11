pragma Singleton

import QtCore
import Salesperson

Settings {
    category: "Salesperson"
    property int salespersonId
    property int salespersonTypeId
    property string salespersonTypeName
    property int creditAvailable
    property int creditConsumed
    property int creditLimit

    function entity(): salesperson {
        return {
            salespersonId,
            salespersonType: salespersonTypeId,
            creditLimit,
            creditConsumed,
            creditAvailable
        }
    }
}
