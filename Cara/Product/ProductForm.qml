import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Utils
import Cara.Shopping
import "qrc:/inventory"
import "product_helper.js" as ProductJS

Page {
    id: root

    readonly property T.StackView myStack: StackView.view
    property int productId
    property int categoryId
    property int sellPrice
    property int buyPrice
    property string description
    property string name
    property var images
    property string mode: "add"
    property var product

    header: TitleHeader {
        title: root.mode == "add" ? "Agregar nuevo producto" : "Editar producto"
        btnLoader.sourceComponent: RowLayout {
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
                        onTriggered: root.myStack.pushItem("Cara/Product/ProductPage.qml", {productId, product})
                    }

                    MenuItem {
                        text: "Ver reseñas"
                        onTriggered: root.myStack.pushItem("Cara/Review/ReviewsPage.qml", {productId})
                    }
                }
            }
        }

        onBackClicked: myStack.pop()
    }

    Component.onCompleted: {
        internal.setOldProperties();

        if (root.mode === "add" || product.images.length === 0) {
            // pictureModel.append({image_url: Qt.url(internal.picturePlaceHolder)});
            // product.images = pictureModel;
            product.images = [{imageUrl: internal.picturePlaceHolder}];
        }
        console.log(JSON.stringify(product.images))
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 1

        BusyIndicator {}

        ScrollView {
            contentWidth: availableWidth

            ColumnLayout {
                anchors.centerIn: parent
                width: root.width*.9
                spacing: 10

                ProductItem {
                    id: productItem
                    images: product.images
                    deleteBtn.visible: product.images.length > 1
                    favoriteBtn.visible: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onDeletePictureClicked: index => root.handlePictureDeleted(index)
                }

                MenuSeparator { Layout.alignment: Qt.AlignCenter }

                FileDialog {
                    id: imgPicker
                    currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                    onAccepted: root.handlePictureSelected(imgPicker.selectedFile)
                }

                TextField {
                    id: productName
                    text: name
                    placeholderText: "Nombre del producto"
                    Layout.fillWidth: true
                }

                TextField {
                    id: descriptionField
                    text: description
                    placeholderText: "Descripcion del producto"
                    Layout.fillWidth: true
                }

                TextField {
                    id: priceBuy
                    text: buyPrice
                    placeholderText: "Precio de compra"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                TextField {
                    id: priceSell
                    text: sellPrice
                    placeholderText: "Precio de venta"
                    Layout.fillWidth: true
                    inputMethodHints: Qt.ImhDigitsOnly
                }

                CategoryComboBox {
                    id: categoryCB
                    Layout.preferredWidth: root.width/2
                    currentIndex: categoryId - 1
                }

                RowLayout {
                    Button {
                        visible: mode !== "add"
                        text: "Ver en inventario"
                        onClicked: openProductInInventory()
                    }

                    Button {
                        text: mode === "add" ? "Agregar" : "Guardar"
                        Layout.fillWidth: true
                        Material.background: Material.Green
                        Material.foreground: "white"
                        onClicked: mode === "add" ? createProduct() : updateProduct()
                    }
                }
            }
        }
    }

    CatalogueRepository {
        id: catalogueRepository
        onProductAdded: data => {
            popup.state = "success";
            ProductManager.modelAppend(JSON.parse(data).product);
        }
        onPictureAdded: data => {
            popup.state = "success";
            console.log(data);
            const pictures = ProductJS.getFixedImagesArr(JSON.parse(data).images);

            console.log(JSON.stringify(pictures, null, 2))
            product.images = pictures;
        }
        onProductEdited: {
            popup.state = "success";
            updateCatalogue();
        }
        onPictureDeleted: {
            popup.state = "success";
            const filteredImages = product.images.filter(item => item.id !== internal.pictureId);
            console.log("to exclude: "+internal.pictureId)
            console.log(JSON.stringify(filteredImages))
            product.images = filteredImages;
        }
        onErrorOccurred: err => {
            console.log(err);
            popup.state = "error";
        }
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
        readonly property url picturePlaceHolder: "cara-logo-v1.png"
        property int pictureId: -1
        property string oldName
        property string oldDescription
        property int oldSellPrice
        property int oldBuyPrice
        property int oldCategoryId

        function setOldProperties() {
            internal.oldName = root.name;
            internal.oldDescription = root.description;
            internal.oldSellPrice = root.sellPrice;
            internal.oldBuyPrice = root.buyPrice;
            internal.oldCategoryId = root.categoryId;
        }
    }

    function openBusyPopup() {
        popup.state = "busy";
        popup.open();
    }

    function closeBusyPopup() {
        popup.state = "";
        popup.close();
    }

    function createProduct() {
        openBusyPopup();
        const productJson = {
            name: productName.text,
            description: descriptionField.text,
            buy_price: priceBuy.text,
            sell_price: priceSell.text,
            images: root.getPictureList(),
            category_id: categoryCB.currentValue
        };
        print(JSON.stringify(productJson, null, 2));
        catalogueRepository.addProduct(productJson);
    }

    function updateProduct() {
        openBusyPopup();
        const productJson = {
            id: productId,
            name: productName.text,
            description: descriptionField.text,
            buy_price: priceBuy.text,
            sell_price: priceSell.text,
            category_id: categoryCB.currentValue
        };

        print(JSON.stringify(productJson, null, 2));
        catalogueRepository.editProduct(productJson);
    }

    function updateCatalogue() {
        const product = ProductManager.findById(productId);
        product.name = productName.text;
        product.description = descriptionField.text;
        product.buy_price = priceBuy.text;
        product.sell_price = priceSell.text;
    }

    function openProductInInventory() {
        const adminId = settings.value("userId");
        inventoryAPI.fetchByAdminIdAndProductId(adminId, productId);
    }

    function getPictureList(): var {
        const pictures = [];
        for (let i = 0; i < product.images.count; ++i) {
            pictures.push({image_url: product.images.get(i).image_url});
        }
        return pictures;
    }

    function handlePictureSelected(filename: url) {
        if (filename.toString() === "") {
            console.log("handlePictureSelected:", filename);
            return;
        }

        if (root.mode === "add") {
            if (product.images.get(0).image_url === internal.picturePlaceHolder)
                product.images.clear();

            product.images.append({image_url: filename});
            return;
        }

        // Edit mode
        openBusyPopup();
        catalogueRepository.addPicture(root.productId, [{image_url: filename}]);
    }

    function handlePictureDeleted(index: int) {
        try {
            if (root.mode === "add") {
                product.images.splice(index, 1);
                return;
            }

            // Deleting an uploaded pic.
            openBusyPopup();
            console.log("index:", index);
            console.log(JSON.stringify(product.images))
            const pictureId = product.images[index].id;
            internal.pictureId = pictureId;
            catalogueRepository.deletePicture(pictureId);
        } catch (err) {
            console.log(err);
            popup.state = "error";
        }
    }
}
