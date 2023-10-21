import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core
import entities as Entity
import "qrc:/singleton"
import "qrc:/utils"
import "qrc:/reviews"
import "qrc:/inventory"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView ? stackView.bar : null
    readonly property bool userIsAdmin: CurrentUser.isAdmin
    readonly property int customerId: TheCurrentUser.customerId
    readonly property bool hasStocks: inStock > 0
    readonly property var editableProduct: {
        "name": [productRating.nameText, api.patchName],
        "sell_price": [parseInt(productRating.price), api.patchSellPrice],
        "description": [productRating.description, api.patchDescription],
    }
    readonly property var product: {
        "name": name,
        "sell_price": sell_price,
        "description": description,
    }

    Entity.Product {
        id: detailsEntity
        name: root.name
        price: root.sell_price
        description: root.description
        productId: root.productId
        imageUrl: root.image_url
    }
    readonly property var cart: CartBusiness

    required property int productId
    required property string name
    required property int sell_price
    required property string description
    required property string image_url
    property int inStock
    property int rating

    header: TitleHeader {
        title: "Producto"
        onBackClicked: root.stackView.pop()
    }

    Component.onCompleted: inventoryApi.fetchByProductId(productId)

    BusyStackLayout {
        id: stackLayout
        anchors.fill: parent

        ScrollView {
            contentWidth: root.width
            contentHeight: columnLayout.height

            ColumnLayout {
                id: columnLayout
                width: parent.width*.85
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }
                spacing: 20

                Product {
                    id: productRating

                    onImageReady: stackLayout.currentIndex = 1
                    onUpdateStocksClicked: updateInStock.open()

                    Layout.fillWidth: true
                    Layout.minimumHeight: childrenRect.height
                    Layout.fillHeight: true
                    Layout.topMargin: 40
                    imgSrc: image_url
                    nameText: name
                    inStock: root.inStock
                    canModify: userIsAdmin
                    price: sell_price
                    description: root.description
                }

                Button {
                    Layout.fillWidth: true
                    text: userIsAdmin ? "Actualizar" : "Comprar"
                    enabled: userIsAdmin || hasStocks
                    onClicked: {
                        if (!userIsAdmin) {
                            if (cart.isProductInCart(detailsEntity))
                                message.show("El producto ya se encuentra en el carrito!");
                            else {
                                cart.addtoCart(detailsEntity);
                                message.show("¡Producto agregado al carrito!");
                            }
                            return;
                        }

                        Object.keys(editableProduct).forEach(key => {
                            const data = editableProduct[key][0];
                            const callable = editableProduct[key][1];

                            if (data !== product[key])
                                callable(data);
                        });
                    }
                    font.pointSize: 12
                }

                Label {
                    id: availableStockLocation
                    text: "Disponible en: "
                    wrapMode: Label.Wrap
                    font.pointSize: 12
                    Layout.fillWidth: true
                }

                ReviewSection {
                    id: reviewSection
                    Layout.fillWidth: true
                }
            }
        }
    }

    Message { id: message }

    StockPopup {
        id: updateInStock
        onOkClicked: quantity => api.patchStock(quantity, () => {
            updateInStock.stackLayout.currentIndex = 1;
            root.inStock = inStock;
        }, () => updateInStock.stackLayout.currentIndex = 2)
    }

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
                    locations += `${inventory.warehouse.name}, `
            }

            locations = locations ? locations.slice(0, locations.length - 2) : "-";
            availableStockLocation.text += locations;

            inStock = totalStock;
        }
    }
}
