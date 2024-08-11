import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Utils
import Cara.Shopping
import Cara.Product
import "qrc:/inventory"
import "product_helper.js" as ProductJS

ProductForm {
    id: root

    required model

    title: "Editar producto"
    actionButtonComponent: Button {
        id: actionButton
        text: "Guardar"
        enabled: !root.same

        Material.background: Material.Green
        Material.foreground: "white"
        onClicked: root.updateProduct()
    }

    buttonComponent: RowLayout {
        id: rowLayout
        ToolButton {
            icon.source: "add_a_photo.svg"
            onClicked: imgPicker.open()
        }

        MoreVertButton {
            onClicked: menu.visible = !menu.visible
            Menu {
                id: menu
                y: parent.height
                rightMargin: 5

                MenuItem {
                    text: "Vista de producto"
                    onTriggered: root.myStack.pushItem("Cara/Product/ProductPage.qml", {productId: model.productId, product})
                }

                MenuItem {
                    text: "Ver reseñas"
                    onTriggered: root.myStack.pushItem("Cara/Review/ReviewsPage.qml", {productId: model.productId})
                }
            }
        }
    }

    pricingLoader.sourceComponent: PricingItem {}

    onPictureAccepted: filename => root.handlePictureSelected(filename)
    onDeletePictureClicked: index => root.handlePictureDeleted(index)

    component PricingItem : GroupBox {
        title: "Precio actual"
        ColumnLayout {
            anchors.fill: parent

            Label {
                text: `<span style="font-size: 11pt;">RD$</span><b>${model.price}</b>`
                horizontalAlignment: Label.AlignHCenter
                textFormat: Label.RichText
                font.pointSize: 14
                Layout.fillWidth: true
            }

            Button {
                id: updatePriceBtn
                text: "Actualizar"
                onClicked: root.myStack.pushItem("qrc:/qt/qml/Cara/Product/PriceHistoryPage.qml", {productId: model.productId})
                Layout.fillWidth: true
            }
        }
    }

    CatalogueRepository {
        id: catalogueRepository
        onPictureAdded: data => {
            popup.state = "success";
            console.log(data);
            const pictures = ProductJS.getFixedImagesArr(JSON.parse(data).images);

            console.log(JSON.stringify(pictures, null, 2))
            root.model.images = pictures;
        }
        onProductEdited: {
            popup.state = "success";
            updateCatalogue();
        }
        onPictureDeleted: {
            popup.state = "success";
            const filteredImages = model.images.filter(item => item.id !== internal.pictureId);
            console.log("to exclude: "+internal.pictureId)
            console.log(JSON.stringify(filteredImages))
            model.images = filteredImages;
        }
        onErrorOccurred: err => root.errorHandler(err)
    }

    StatusPopup {
        id: popup
        width: root.width*.8
        height: root.height/2
    }

    InventoryAPI {
        id: inventoryAPI
        onFetchedByAdminIdAndProductId: inventory => {
            myStack.push("qrc:/inventory/InventoryForm.qml", {
                inventoryId: inventory.id,
                product: inventory.product,
                warehouse: inventory.warehouse,
                minimunStockLevel: inventory.minimum_stock_level,
                maximunStockLevel: inventory.maximum_stock_level,
                reorderPoint: inventory.reorder_point,
                quantityAvailable: inventory.quantity_available
            });
        }
    }

    QtObject {
        id: internal
        property int pictureId: -1
    }

    function openBusyPopup() {
        popup.state = "busy";
        popup.open();
    }

    function closeBusyPopup() {
        popup.state = "";
        popup.close();
    }

    function updateProduct() {
        openBusyPopup();

        const productjson = root.toJson();
        productjson["id"] = model.productId;

        catalogueRepository.editProduct(productjson);
    }

    function updateCatalogue() {
        console.log(root.model,null,2)
        //console.log(JSON.stringify(root.model,null,2))
        console.log(root.model.productName)
        console.log(root.model.description)
        //ProductManager.updateProduct(root.model);
        root.model.productName = root.nameField.text;
        root.model.description = root.descriptionField.text;
        root.model.categoryId = root.categoryCB.currentValue;
        console.log(root.model.productName)
    }

    function openProductInInventory() {
        const adminId = settings.value("userId");
        inventoryAPI.fetchByAdminIdAndProductId(adminId, model.productId);
    }

    function handlePictureSelected(filename: url) {
        root._handlePictureSelected(filename);

        openBusyPopup();
        catalogueRepository.addPicture(model.productId, [{imageUrl: filename}]);
    }

    function handlePictureDeleted(index: int) {
        try {
            console.log("EditProductPage: handlePictureDeleted:")
            // Deleting an uploaded pic.
            openBusyPopup();
            console.log("index:", index);
            const pictureId = model.images[index].id;
            internal.pictureId = pictureId;
            console.log("internal.pictureId:",internal.pictureId)
            catalogueRepository.deletePicture(pictureId);
        } catch (err) {
            console.log(err);
            popup.state = "error";
        }
    }

    function getProduct(): product {
        return {

        }
    }
}
