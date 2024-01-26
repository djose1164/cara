import QtQuick

AddressSelector {
    id: root

    text: "Municipio"
    Component.onCompleted: addressApi.fetchMunicipalities(1, 1);

    AddressAPI {
        id: addressApi
        onMunicipalitiesFetched: municipalities => municipalities.forEach(e => model.append(e))
    }
}
