import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara
import Cara.Utils
import Cara.Shopping
import Cara.User
import Cara.Review
import "qrc:/inventory"

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    readonly property int userId: UserSettings.userId
    required property int productId
    required property var product
    property int inStock

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
        inventoryApi.fetchByProductId(root.productId);
        reviewModel.fetchReviewsByProductId(root.productId);
        ReviewController.canCustomerReview(UserSettings.customerId, root.productId);
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
            spacing: 40

            ShopProduct {
                id: shopProduct
                sellPrice: root.product.sell_price
                stock: root.inStock
                favoriteProduct: favoriteResource
                productForSale: root.product
                Layout.fillWidth: true
                onBuyClicked: product => handleBuyClicked(product)
            }

            CommentRating {
                id: commentRating
                productId: root.productId
                Layout.fillWidth: true
            }

            Label {
                text: "Reseñas y calificaciones"
                font.bold: true
                font.pointSize: 14
                Layout.topMargin: 10
            }

            Repeater {
                model: reviewModel
                CustomerReview {
                    required reviewText
                    required reviewer
                    required rating
                    required createdAt
                    required customerId

                    visible: UserSettings.customerId === 0 || UserSettings.customerId !== customerId
                    Layout.fillWidth: true
                }
            }

            Item { height: 1 }
        }
    }

    ReviewModel {
        id: reviewModel
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
        readonly property int margin: Math.max(scrollView.effectiveScrollBarWidth, 10)
        property string shortDescription
        property favorite fav
    }

    function handleBuyClicked(productToBuy: product) {
        if (CartController.isProductInCart(productToBuy)) {
            message.show("Este producto ya se encuentra en el carrito");
            return;
        }

        CartController.addtoCart(productToBuy);
    }
}
