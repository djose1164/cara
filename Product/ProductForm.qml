import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Utils
import Shopping
import "qrc:/inventory"

Page {
    id: root

    readonly property var myStack: StackView.view
    property int productId
    property int categoryId
    property int sellPrice
    property int buyPrice
    property string description
    property string name
    property var images
    property string mode: "add"

    header: TitleHeader {
        title: root.mode == "add" ? "Agregar nuevo producto" : "Editar producto"
        btnLoader.sourceComponent: ToolButton {
            icon.source: "add_a_photo.svg"
            onClicked: imgPicker.open()
        }

        onBackClicked: myStack.pop()
    }

    Component.onCompleted: {
        if (root.mode === "add") {
            pictureModel.append({image_url: Qt.url(internal.picturePlaceHolder)});
            root.images = pictureModel;
        }
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
                    images: root.images
                    deleteBtn.visible: root.images.count > 1
                    favoriteBtn.visible: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onDeletePictureClicked: index => root.handlePictureDeleted(index)
                }

                MenuSeparator { Layout.alignment: Qt.AlignCenter }

                ImagePicker {
                    id: imgPicker
                    visible: false
                    Layout.preferredWidth: root.width/2
                    Layout.preferredHeight: root.width/2
                    onFileAccepted: filename => root.handlePictureSelected(filename)
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

    ListModel {
        id: pictureModel
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
            const pictures = JSON.parse(data).images;
            //root.images.append(pictures);
            root.images.clear();
            root.images.append(pictures);
        }
        onProductEdited: {
            popup.state = "success";
            updateCatalogue();
        }
        onPictureDeleted: {
            popup.state = "success";
            root.images.remove(internal.pictureIndex);
        }
        onErrorOccurred: popup.state = "error"
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
        property int pictureIndex: -1
        readonly property url picturePlaceHolder: "cara-logo-v1.png"
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

        if (root.images.count > 0)
            productJson.images = root.getPictureList();

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
        for (let i = 0; i < root.images.count; ++i) {
            pictures.push({image_url: root.images.get(i).image_url});
        }
        return pictures;
    }

    function handlePictureSelected(filename: url) {
        if (root.mode === "add") {
            if (root.images.get(0).image_url === internal.picturePlaceHolder)
                root.images.clear();

            root.images.append({image_url: filename});
            return;
        }

        // Edit mode
        openBusyPopup();
        catalogueRepository.addPicture(root.productId, [{image_url: filename}]);
    }

    function handlePictureDeleted(index: int) {
        try {
            if (root.mode === "add") {
                root.images.remove(index);
                return;
            }

            // Deleting an uploaded pic.
            openBusyPopup();
            internal.pictureIndex = index;
            const pictureId = root.images.get(index).id;
            catalogueRepository.deletePicture(pictureId);
        } catch (err) {
            popup.state = "error";
        }
    }
}
