import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Cara.Shopping
import Cara.Product
import Cara.Inventory
import "product_helper.js" as ProductJS

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

    onPictureAccepted: filename => root.handlePictureSelected(filename)

    component AddPhotoBtn : ToolButton {
        icon.source: "add_a_photo.svg"
        onClicked: imgPicker.open()
    }

    component PricingItem : TextField {
        id: priceSell
        placeholderText: "Precio de venta"
        inputMethodHints: Qt.ImhDigitsOnly

        function toJson() {
            return [
                {
                    price: parseInt(priceSell.text),
                    price_type: "sell"
                }
            ];
        }
    }

    CatalogueRepository {
        id: catalogueRepository
        onProductAdded: data => {
            popup.close();
            const newProduct = ProductManager.productFromJson(JSON.parse(data).product);
            // ProductManager.modelAppend(newProduct);
            root.productModel.append(newProduct);
            successDialog.open();
        }

        onPictureAdded: data => {
            popup.state = "success";
            console.log(data);
            const pictures = ProductJS.getFixedImagesArr(JSON.parse(data).images);

            console.log(JSON.stringify(pictures, null, 2))
            product.images = pictures;
        }

        onErrorOccurred: err => root.errorHandler(err)
    }

    MessageDialog {
        id: successDialog
        title: "Product agregado"
        text: "¡El producto ha sido agregado al catálogo exitosamente!"
        onAccepted: {
            root.myStack.popCurrentItem();
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
            productFormJson["price_history"] = pricingItem.toJson();
            print(JSON.stringify(productFormJson, null, 2));

            //const productJson = JSON.parse(newProduct.toJson());
            catalogueRepository.addProduct(productFormJson);
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
        const categoryId = productFormJson.category_id;
        if (isNaN(categoryId) || categoryId < 1)
            throw "Necesitas seleccionar una categoría."
    }
}
