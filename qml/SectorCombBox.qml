import QtQuick
import QtQuick.Controls

AddressSelector {
    id: root

    text: "Sector"
    Component.onCompleted: addressApi.fetchSectors(1, 1, 1);

    AddressAPI {
        id: addressApi
        onSectorsFetched: sectors => model.append(sectors)
    }
}
