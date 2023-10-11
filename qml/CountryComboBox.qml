import QtQuick
import QtQuick.Controls

AddressSelector {
    id: root

    text: "Pais"
    Component.onCompleted: addressApi.fetchCountries();

    AddressAPI {
        id: addressApi
        onCountriesFetched: countries => model.append(countries)
    }
}
