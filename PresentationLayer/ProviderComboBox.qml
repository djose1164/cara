import QtQuick
import QtQuick.Controls

ComboBox {
    id: root

    Component.onCompleted: providerApi.fetchProviders()

    model:  providersModel
    textRole: "name"
    valueRole: "id"
    currentIndex: 0

    ListModel { id: providersModel }

    ProviderAPI {
        id: providerApi
        onProvidersFetched: providers => providersModel.append(providers)
    }
}
