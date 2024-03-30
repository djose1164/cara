import QtQuick
import QtQuick.Controls
import Shopping as Catalogue

ComboBox {
    id: root
    width: 300
    height: 30
    textRole: "name"
    valueRole: "id"
    model: listModel
    displayText: currentIndex === -1 ? "Categoria" : currentText
    Component.onCompleted: catalogueRepository.fetchCategories()

    ListModel { id: listModel }

    Catalogue.CatalogueRepository {
        id: catalogueRepository
        onCategoriesFetched: data => listModel.append(JSON.parse(data).categories)
    }
}
