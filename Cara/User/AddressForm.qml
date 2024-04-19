import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils
import "qrc:/address"

GroupBox {
    id: root

    readonly property var address: {
        "country_id": country.comboBox.currentValue,
        "province_id": province.comboBox.currentValue,
        "municipality_id": municipality.comboBox.currentValue,
        "sector_id": sector.comboBox.currentValue,
        "street": street.text,
        "house_number": parseInt(house_num.text)
    }
    readonly property bool same: internal.oldCountryId === country.comboBox.currentValue
                                 && internal.oldProvinceId === province.comboBox.currentValue
                                 && internal.oldMunicipalityId === municipality.comboBox.currentValue
                                 && internal.oldSectorId === sector.comboBox.currentValue
                                 && internal.oldStreet === street.text
                                 && internal.oldHouseNumber === parseInt(house_num.text)
    title: "Direccion"

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        CountryComboBox {
            id: country
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        ProvinceComboBox {
            id: province
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        MunicipalityComboBox {
            id: municipality
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        SectorCombBox {
            id: sector
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        TextField {
            id: street
            placeholderText: "Calle"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        NumberTextField {
            id: house_num
            placeholderText: "Numero de casa"
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
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
        country.comboBox.currentIndex = address.country_id - 1;
        province.comboBox.currentIndex = address.province_id - 1;
        municipality.comboBox.currentIndex = address.municipality_id - 1;
        sector.comboBox.currentIndex = address.sector_id - 1;
        street.text = address.street;
        house_num.text = address.house_number;

        fillOldProperties(address);
    }

    function fillOldProperties(address: var) {
        internal.oldCountryId = address.country_id;
        internal.oldProvinceId = address.province_id;
        internal.oldMunicipalityId = address.municipality_id;
        internal.oldSectorId = address.sector_id;
        internal.oldStreet = address.street;
        internal.oldHouseNumber = house_num.text;
    }

    function resetSame() {
        fillOldProperties(root.address);
    }
}
