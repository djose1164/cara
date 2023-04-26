import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/utils"
import "qrc:/reviews"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView ? stackView.bar : null
    readonly property bool userIsAdmin: TheCurrentUser.isAdmin
    readonly property int customerId: TheCurrentUser.customerId
    readonly property var editableProduct: {
        "name": [productRating.nameText, api.patchName],
        "sell_price": [parseInt(sellPriceField.text), api.patchSellPrice],
        "description": [descriptionField.text, api.patchDescription],
    }
    readonly property var product: {
        "name": name,
        "sell_price": sell_price,
        "description": description,
    }
    readonly property var emptyReview: {
        "customer_id": customerId
    }

    required property int productId
    required property string name
    required property int sell_price
    required property string description
    required property string image_url
    required property int inStock
    property int rating

    header: TitleHeader {
        title: "Producto"
        stackView: root.stackView
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        CenterBusyIndicator {}

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

                ProductRating {
                    id: productRating
                    Layout.fillWidth: true
                    Layout.minimumHeight: childrenRect.height
                    Layout.fillHeight: true
                    Layout.topMargin: 40
                    imgSrc: image_url
                    selectedStars: root.rating
                    nameText: name
                    nameEnabled: userIsAdmin
                }

                ProductStocks {
                    Layout.bottomMargin: 20
                    onUpdateClicked: updateInStock.open()
                    inStock: root.inStock
                    canUpdate: userIsAdmin
                }

                RowLayout {
                    Label {
                        text: `Precio: RD$`
                        font.pointSize: 12
                    }
                    TextEdit {
                        id: sellPriceField
                        text: sell_price
                        enabled: userIsAdmin
                    }
                }

                TextEdit {
                    id: descriptionField
                    Layout.fillWidth: true
                    text: description
                    enabled: userIsAdmin
                    visible: description
                }

                Button {
                    Layout.fillWidth: true
                    text: userIsAdmin ? "Actualizar" : "Comprar"
                    onClicked: {
                        if (!userIsAdmin) {
                            bar.currentIndex = 1;
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
                    text: "Reseñas"
                    font.bold: true
                    font.pointSize: 16
                }

                Reviews {
                    id: reviews
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: reviewsModel.count ? root.height*.4 : root.height*.25
                    Layout.topMargin: 20
                    model: reviewsModel
                }
            }
        }
    }

    StockPopup {
        id: updateInStock
        onOkClicked: quantity => api.patchStock(quantity, () => {
            updateInStock.stackLayout.currentIndex = 1;
            root.inStock = inStock;
        }, () => updateInStock.stackLayout.currentIndex = 2)
    }

    ProductApi { id: api }

    ListModel { id: reviewsModel }

    ReviewApi {
        id: reviewApi
        onAllReviewsFetched: r => {
            const ratings = r.map(item => item.rating);
            setProductRating(ratings);
            reviewsModel.append(r.filter(item => item.customer_id !== customerId));
            const res = r.find(item => item.customer_id === customerId);
            if (res)
                reviewsModel.insert(0, res);
        }
        Component.onCompleted: fetchAllReviews(productId)
    }

    function setProductRating(ratings) {
        const sum = ratings.reduce((total, num) => total + num, 0);
        root.rating = Math.round(sum / ratings.length);
    }
}
