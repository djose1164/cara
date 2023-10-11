import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core as Core
import catalogue as Catalogue
import "qrc:/catalogue" as Catalogue
import "qrc:/utils"
import "qrc:/main"
import "qrc:/singleton"
import "qrc:/inventory"

Page {
    id: root

    readonly property var myStack: StackView.view
    required property int productId
    required property int categoryId
    required property int sellPrice
    required property int buyPrice
    required property string description
    required property string name
    required property url imageUrl
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
                    Layout.fillWidth: true
                    Button {
                        text: "Nueva categoria"
                    }

                    Button {
                        text: mode === "add" ? "Agregar" : "Guardar"
                        Layout.fillWidth: true
                        Material.background: Material.Green
                        Material.foreground: "white"
                        onClicked: mode === "add" ? createProduct() : updateProduct()
                    }
                }

                Button {
                    text: "Ver en inventario"
                    Layout.fillWidth: true
                    onClicked: openProductInInventory()
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog
        text: mode === "add" ? "Producto guardado." : "Producto editado."
        informativeText: "Puedes chequear el producto desde Inventario.";
        onButtonClicked: close()
    }

    ProductRequest {
        id: productRequest
        request.onSucceeded: {
            ProductManager.updateModel();
            stackLayout.currentIndex = 1;
            messageDialog.open();
        }

        request.onFailed: resp => {
            messageDialog.text = "No se ha podido guardar el producto.";
            messageDialog.informativeText = resp.message;
            messageDialog.open();
            stackLayout.currentIndex = 1;
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
                minimunStockLevel: inventory.minimun_stock_level,
                maximunStockLevel: inventory.maximun_stock_level,
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
            imageUrl: imgPicker.imagePath,
            category_id: categoryCB.currentValue
        };
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
        const adminId = Core.CurrentUser.user.userId;
        inventoryAPI.fetchByAdminIdAndProductId(adminId, productId);
    }
}
