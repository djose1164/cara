import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.User
import Cara.Shopping
import Cara.ProductInventory
import Cara.Network
import "qrc:/qt/qml/Cara/Product/product_helper.js" as ProductJS

Item {
    id: root

    required property FavoriteProduct favoriteProduct
    required property var productForSale

    readonly property  int productId: root.productForSale.id
    readonly property int userId: UserSettings.userId

    readonly property alias selectedQty: priceSpinBox.selectedQty
    readonly property alias buyButton: buyButton

    property list<string> locations
    property int sellPrice
    property int stock
    property int buyButtonState: ShopProduct.Ok

    signal buyClicked(productToBuy: product)

    enum BuyButtonState {
        OutOfStocks,
        AlreadyInCart,
        Ok
    }

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 20

        ProductItem {
            id: productItem
            images: ProductJS.getFixedImagesArr(root.productForSale.images)
            name: root.productForSale.name
            deleteBtn.visible: UserSettings.userTypeId !== UserEntity.Customer
            Layout.fillWidth: true
            onFavoriteClicked: like => handleFavClicked(like)
        }

        PriceSpinBox {
            id: priceSpinBox
            enabled: internal.canBuy
            to: internal.topLimit
            unitPrice: root.sellPrice
            Layout.fillWidth: true
        }

        Button {
            id: buyButton
            text: switch (root.buyButtonState) {
            case ShopProduct.OutOfStocks:
                return "Más uninades en camino";
            case ShopProduct.AlreadyInCart:
                return "¡Producto agregado al carrito!"
            case ShopProduct.Ok:
                return "Agregar al Carrito";
            }

            enabled: root.buyButtonState === ShopProduct.Ok
            Layout.fillWidth: true
            onClicked: {
                const productObj = {
                    name: root.productForSale.name,
                    price: root.productForSale.sell_price,
                    productId: root.productId,
                    imageUrl: Api.uploadsPath + root.productForSale.images[0].image_url,
                    selectedQty: root.selectedQty
                };
                print(JSON.stringify(productObj, null, 2));
                const productToBuy = root.createProduct(productObj);
                root.buyClicked(productToBuy);
            }
        }

        Label {
            text: "Cantidad disponible: " + root.stock
        }

        Label {
            id: availableStockLocation
            text: root.getAvailableLocations()
            wrapMode: Label.Wrap
            font.pointSize: 12
            Layout.fillWidth: true
        }
    }

    Connections {
        target: root.favoriteProduct

        function onResourceFetched(resp) {
            const favorites = JSON.parse(JSON.parse(resp).favorites);
            const func = fav => {
                internal.fav = createFavorite({
                                                  favoriteId: fav.id,
                                                  userId: fav.user_id,
                                                  productId: fav.product_id
                                              });
                if (fav.product_id === root.productId) {
                    productItem.favoriteBtn.like = true;
                    return true;
                }
                return false;
            }
            for (let fav of favorites)
                if (func(fav))
                    break;
        }

        function onResourceCreated(resp) {
            const fav = JSON.parse(resp).favorite;
            internal.fav = root.createFavorite({
                                                   favoriteId: fav.id,
                                                   userId: fav.user_id,
                                                   productId: fav.product_id
                                               });
        }
    }

    QtObject {
        id: internal
        readonly property bool canBuy: root.stock > 0
        readonly property int topLimit: canBuy ? root.stock : 1
        property favorite fav
    }

    function getAvailableLocations(): string {
        const template = "Disponible en: %1";

        const length = root.locations.length;
        if (length === 0)
            return template.arg("-");

        let finalLocations = "";
        for (let i = 0; i < length; ++i) {
            const location = root.locations[i];
            if ((i + 1) === length)
                finalLocations += `${location}`;
            else
                finalLocations += `${location}, `;
        }
        print(template.arg(finalLocations))
        return template.arg(finalLocations);
    }

    function handleFavClicked(like: bool) {
        const favorite = createFavorite({userId: root.userId, productId: root.productId});
        if (like)
            favoriteProduct.add(favorite);
        else
            favoriteProduct.remove(internal.fav);
    }

    function createFavorite({userId = -1, productId = -1, favoriteId = -1}): favorite {
        return {
            userId,
            productId,
            favoriteId
        };
    }

    function createProduct({productId, price, imageUrl, name, selectedQty}): product {
        return {
            productId,
            price,
            imageUrl,
            name,
            selectedQty
        };
    }
}
