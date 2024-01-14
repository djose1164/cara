import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core as Core
import catalogue as Catalogue
import User
import "qrc:/utils"
import "qrc:/main"
import "qrc:/singleton"
import "qrc:/inventory"

Page {
    id: root

    readonly property var myStack: StackView.view
    readonly property var user: UserSession.user
    property int productId
    property int categoryId
    property int sellPrice
    property int buyPrice
    property string description
    property string name
    property url imageUrl
    property string mode: "add"

    header: TitleHeader {
        title: "Agregar nuevo producto"
        onBackClicked: myStack.pop()
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

                ImagePicker {
                    id: imgPicker
                    Layout.preferredWidth: root.width/2
                    Layout.preferredHeight: root.width/2
                    imagePath: imageUrl
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

                Catalogue.CategoryComboBox {
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

    Catalogue.CatalogueRepository {
        id: catalogueRepository
        onProductAdded: data => {
            popup.state = "success";
            ProductManager.modelAppend(JSON.parse(data).product);
        }
        onProductEdited: {
            popup.state = "success";
            updateCatalogue();
        }
        onErrorOccurred: popup.state = "error"
    }

    Catalogue.StatusPopup {
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
            image_url: imgPicker.imagePath,
            category_id: categoryCB.currentValue
        };
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
        const adminId = user.userId;
        inventoryAPI.fetchByAdminIdAndProductId(adminId, productId);
    }
}
