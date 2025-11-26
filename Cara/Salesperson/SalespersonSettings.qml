pragma Singleton

import QtCore
import Cara.Salesperson

Settings {
    id: root

    property int salespersonId
    property int salespersonTypeId
    property string salespersonTypeName
    property int creditAvailable
    property int creditConsumed
    property int creditLimit
    property string warehouseAddress
    property int organizationId

    category: "Salesperson"

    function entity(): salesperson {
        return {
            salespersonId,
            salespersonType: salespersonTypeId,
            creditLimit,
            creditConsumed,
            creditAvailable,
            warehouseAddress,
            organizationId
        }
    }

    function setEntity(sp: salesperson) {
        root.salespersonId = sp.salespersonId;
        root.salespersonTypeId = sp.salespersonType;
        root.salespersonTypeName = sp.salespersonTypeName;
        root.creditAvailable = sp.creditAvailable;
        root.creditConsumed = sp.creditConsumed;
        root.creditLimit = sp.creditLimit;
        root.warehouseAddress = sp.warehouseAddress;
        root.organizationId = sp.organizationId;
        console.log("setEntity: organizationId: ",sp.organizationId)
    }

    function setEntityFromJson(sp) {
        root.salespersonId = sp.id;
        root.salespersonTypeId = sp.salesperson_type_id;
        //root.salespersonTypeName = sp.salespersonTypeName;
        root.creditAvailable = sp.credit_available;
        root.creditConsumed = sp.credit_consumed;
        root.creditLimit = sp.credit_limit;
        // root.warehouseAddress = sp.warehouseAddress;
        root.organizationId = sp.organization.id;
        console.log("setEntity: organizationId: ",sp.organizationId)
    }

    function clear() {
        root.salespersonId = 0;
        root.salespersonTypeId = 0;
        root.salespersonTypeName = "";
        root.creditAvailable = 0;
        root.creditConsumed = 0;
        root.creditLimit = 0;
        root.warehouseAddress = "";
        root.salespersonId = 0;
    }
}
