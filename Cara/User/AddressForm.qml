import QtQuick
import QtQuick.Controls
import "qrc:/address"

GroupBox {
    id: root

    readonly property var address: addressItem.toJson()
    readonly property bool same: internal.oldCountryId === addressItem.countryValue
                                 && internal.oldProvinceId === addressItem.provinceValue
                                 && internal.oldMunicipalityId === addressItem.municipalityValue
                                 && internal.oldSectorId === addressItem.sectorValue
                                 && internal.oldStreet === addressItem.streetText
                                 && internal.oldHouseNumber === parseInt(addressItem.houseNumberText)
    title: "Direccion"

    AddressItem {
        id: addressItem
        anchors.fill: parent
    }

    QtObject {
        id: internal
        property int oldCountryId
        property int oldProvinceId
        property int oldMunicipalityId
        property int oldSectorId
        property string oldStreet
        property int oldHouseNumber
    }

    function fillFields(address: var) {
        addressItem.fillFields(address);

        fillOldProperties(address);
    }

    function fillOldProperties(address: var) {
        internal.oldCountryId = address.country_id;
        internal.oldProvinceId = address.province_id;
        internal.oldMunicipalityId = address.municipality_id;
        internal.oldSectorId = address.sector_id;
        internal.oldStreet = address.street;
        internal.oldHouseNumber = parseInt(addressItem.houseNumberText);
    }

    function resetSame() {
        fillOldProperties(root.address);
    }

    function toJson() {
        return addressItem.toJson();
    }
}
