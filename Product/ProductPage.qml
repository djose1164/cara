import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Utils
import Shopping
import User
import "qrc:/reviews"
import "qrc:/inventory"

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    readonly property TabBar bar: stackView ? stackView.bar : null
    readonly property int userId: UserSettings.userId
    readonly property bool hasStocks: inStock > 0
    readonly property var product_: {
        "name": name,
        "sell_price": sell_price,
        "description": description,
    }

    // Product {
    //     id: detailsEntity
    //     description: root.description
    // }

    readonly property var cart: CartController

    required property int productId
    required property string name
    required property int sell_price
    required property string description
    required property string image_url
    required property var product
    property int inStock
    property int rating

    header: TitleHeader {
        title: internal.titleTemplate.arg(root.title).arg(internal.shortDescription)
        label.textFormat: Label.RichText
        label.font.bold: false
        btnLoader.sourceComponent: MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu
                MenuItem {
                    text: "Mas informacion"
                }
            }
        }

        onBackClicked: root.stackView.pop()
    }

    Component.onCompleted: {
        organizationResource.readById(internal.caraId);
        inventoryApi.fetchByProductId(productId);
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: root.width

        ColumnLayout {
            id: columnLayout
            anchors {
                fill: parent
                topMargin: 10
                bottomMargin: 20
                leftMargin: internal.margin
                rightMargin: internal.margin
            }
            spacing: 20

            ProductItem {
                id: productItem
                images: root.product.images
                name: root.product.name
                Layout.fillWidth: true
                onFavoriteClicked: like => handleFavClicked(like)
            }

            ShopProduct {
                id: shopProduct
                sellPrice: root.product.sell_price
                stock: root.inStock
                Layout.fillWidth: true
                onBuyClicked: handleBuyClicked()
            }

            ReviewSection {
                id: reviewSection
                Layout.fillWidth: true
                Layout.bottomMargin: 10
            }
        }
    }

    Message { id: message }

    ProductApi { id: api }

    InventoryAPI {
        id: inventoryApi
        onFetchedInventoriesByProductId: function(inventories) {
            let totalStock = 0;
            let locations = "";

            for (let inventory of inventories) {
                const qty = inventory.quantity_available;
                totalStock += qty;

                if (qty > 0)
                    shopProduct.locations.push(inventory.warehouse.name)
            }

            inStock = totalStock;
        }
    }

    RestService {
        OrganizationResource {
            id: organizationResource
            path: "organizations"
            onFetched: data => {
                const organization = JSON.parse(data).organization;
                root.title = organization.name;
                internal.shortDescription = organization.short_description;
            }
        }

        FavoriteProduct {
            id: favoriteResource
            path: `users/${root.userId}/favorites`
            onResourceFetched: resp => {
                print(resp);
                const favorites = JSON.parse(JSON.parse(resp).favorites);
                print(favorites)
                print(`length: ${favorites.length}`)
                const func = fav => {
                    internal.fav = createIdentifiedFavorite(fav.id, fav.user_id, fav.product_id);
                    if (fav.product_id === root.product.id) {
                        productItem.favoriteBtn.like = true;
                        return true;
                    }
                    return false;
                }
                for (let fav of favorites)
                    if (func(fav))
                        break;
            }
            onResourceCreated: resp => {
                const fav = JSON.parse(resp).favorite;
                internal.fav = createIdentifiedFavorite(fav.id, fav.user_id, fav.product_id);
            }

            Component.onCompleted: {
                readByCustomerId(root.userId)
            }
        }
    }

    UserController {
        id: userController
    }

    QtObject {
        id: internal
        readonly property int caraId: 1
        readonly property string titleTemplate: `<b>%1</b><br><font size="2">%2</font>`
        readonly property int margin: scrollView.effectiveScrollBarWidth ?? 10
        property string shortDescription
        property favorite fav
    }

    function createFavorite(userId: int, productId: int): favorite {
        return {
            userId,
            productId
        };
    }

    function createIdentifiedFavorite(favoriteId: int, userId: int, productId: int): favorite {
        return {
            userId,
            productId,
            favoriteId
        };
    }

    function handleFavClicked(like: bool) {
        const favorite = createFavorite(root.userId, root.productId);
        if (like)
            favoriteResource.add(favorite);
        else
            favoriteResource.remove(internal.fav);
    }

    function createProduct({productId, price, imageUrl, name}): product {
        return {
            productId,
            price,
            imageUrl,
            name
        };
    }

    function handleBuyClicked() {
        const productObj = {
            name: root.name,
            price: root.sell_price,
            productId: root.productId,
            imageUrl: root.image_url
        };
        const productToBuy = createProduct(productObj);
        if (CartController.isProductInCart(productToBuy)) {
            message.show("Este producto ya se encuentra en el carrito");
            return;
        }

        CartController.addtoCart(productToBuy);
    }
}
