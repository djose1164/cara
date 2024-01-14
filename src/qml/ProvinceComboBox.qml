import QtQuick
import QtQuick.Controls

AddressSelector {
    id: root

    text: "Provincia"
    Component.onCompleted: addressApi.fetchPronvinces(1);

    AddressAPI {
        id: addressApi
        onPronvicesFetched: provinces => model.append(provinces)
    }
}
