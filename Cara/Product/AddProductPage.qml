pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Cara.Product
import Cara.ProductInventory
import Cara.Controls
import Cara.Billing

ProductForm {
    id: root

    required property ProductModel productModel
    property product newProduct: ({images: internal.imagesPlaceHolder})
    property var pictures: internal.imagesPlaceHolder

    model: root.newProduct
    title: "Agregar producto"
    actionButtonComponent: Button {
        text: "Agregar"
        Material.background: Material.Green
        Material.foreground: "white"
        onClicked: root.createProduct()
    }

    buttonComponent: AddPhotoBtn {}
    pricingLoader.sourceComponent: PricingItem {}
    supplierLoader.sourceComponent: BuyPriceSupplierItem {}

    onPictureAccepted: filename => root.handlePictureSelected(filename)

    component AddPhotoBtn : ToolButton {
        icon.source: "add_a_photo.svg"
        onClicked: root.imgPicker.open()
    }

    component PricingItem : TextField {
        id: priceSell
        placeholderText: "Precio de venta"
        inputMethodHints: Qt.ImhDigitsOnly
    }

    component BuyPriceSupplierItem : GroupBox {
        title: "Suplidor y precio de compra"

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            ProviderComboBox {
                id: supplierCB
                Layout.fillWidth: true

            }

            NumberTextField {
                id: priceField
                text: root.model.supplierPrice
                placeholderText: "Precio de compra"
                Layout.fillWidth: true
            }
        }

        function toJson() {
            return {
                supplierId: supplierCB.currentValue,
                supplierPrice: parseInt(priceField.text)
            }
        }
    }

    MessageDialog {
        id: successDialog
        title: "Product agregado"
        text: "¡El producto ha sido agregado al catálogo exitosamente!"
        onAccepted: {
            root.myStack.popCurrentItem();
        }
    }

    Connections {
        target: ProductInventoryController.catalog

        function onAdded() {
            root.popup.close();
            successDialog.open();
        }
    }

    QtObject {
        id: internal
        readonly property string picturePlaceHolder: "cara-logo-v1.png"
        readonly property var imagesPlaceHolder: [{imageUrl: internal.picturePlaceHolder}]
    }

    function createProduct() {
        try {
            validate();
            openBusyPopup();
            const productFormJson = root.toJson();
            productFormJson.price = parseInt((pricingItem as TextField).text);
            Object.assign(productFormJson, supplierItem.toJson());
            print(JSON.stringify(productFormJson, null, 2));

            ProductInventoryController.catalog.addProduct(productFormJson);
        } catch (err) {
            popup.text = err;
            popup.state = "error";
            popup.open();
        }
    }

    function handlePictureSelected(filename: url) {
        root._handlePictureSelected(filename);

        const productImages = root.newProduct.images;
        if (productImages[0].imageUrl === internal.picturePlaceHolder)
            root.newProduct.clearImages();

        root.newProduct.appendImage(filename);
    }

    function validate() {
        const productFormJson = root.toJson();
        const categoryId = productFormJson.categoryId;
        if (isNaN(categoryId) || categoryId < 1)
            throw "Necesitas seleccionar una categoría."
    }
}
