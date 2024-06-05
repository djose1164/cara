import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls
import "qrc:/address"

Item {
    id: root

    default property alias content: columnLayout.data
    property alias countryValue: country.currentValue
    property alias provinceValue: province.currentValue
    property alias municipalityValue: municipality.currentValue
    property alias sectorValue: sector.currentValue
    property alias streetText: street.text
    property alias houseNumberText: house_num.text

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 30

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

    function fillFields(address: var) {
        if (!address)
            throw new Error("address cannot be null.");

        country.currentIndex = address.country_id - 1;
        province.currentIndex = address.province_id - 1;
        municipality.currentIndex = address.municipality_id - 1;
        sector.currentIndex = address.sector_id - 1;
        street.text = address.street;
        house_num.text = address.house_number;
    }

    function validate() {

    }

    function toEntity(): address {
        return toJson();
    }

    function toJson() {
        return {
            "country_id": country.currentValue,
            "province_id": province.currentValue,
            "municipality_id": municipality.currentValue,
            "sector_id": sector.currentValue,
            "street": street.text,
            "house_number": parseInt(house_num.text)
        };
    }
}
