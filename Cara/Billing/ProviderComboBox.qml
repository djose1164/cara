import QtQuick
import QtQuick.Controls

ComboBox {
    id: root

    readonly property bool selected: currentIndex !== -1

    Component.onCompleted: providerApi.fetchProviders()

    model:  providersModel
    textRole: "name"
    valueRole: "id"
    displayText: currentIndex === -1 ? "Seleccionar suplidor" : currentText

    ListModel { id: providersModel }

    ProviderAPI {
        id: providerApi
        onProvidersFetched: providers => providers.forEach(e => providersModel.append(e))
    }
}
