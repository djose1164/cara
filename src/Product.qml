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
        "name": [nameField.text, api.patchName],
        "sell_price": [parseInt(sellPriceField.text), api.patchSellPrice],
        "description": [descriptionField.text, api.patchDescription],
    }
    readonly property var product: {
        "name": name,
        "sell_price": sell_price,
        "description": description,
    }

    required property int productId
    required property string name
    required property int sell_price
    required property string description
    required property string image_url
    required property int inStock
    required property int rating

    header: TitleHeader {
        title: "Producto"
        stackView: root.stackView
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Item {
            BusyIndicator {
                anchors.centerIn: parent
            }
        }

        ScrollView {
            contentWidth: root.width
            contentHeight: columnLayout.height

            ColumnLayout {
                id: columnLayout
                width: parent.width*.8
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                ProductImage {
                    id: img
                    Layout.alignment: Qt.AlignHCenter
                    source: image_url
                    sourceSize: `${root.width*.9}x${root.height*.4}`
                }

                StarRating {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: root.height*.1
                    Component.onCompleted: if (rating) selectedStars = rating
                }

                TextEdit {
                    id: nameField
                    text: name
                    font.pointSize: 14
                    enabled: userIsAdmin
                }

                RowLayout {
                    Label {
                        text: inStock === 1 ? `¡Solo queda ${inStock}!`
                                            : inStock ? `¡Solo quedan ${inStock}!`
                                                      : `¡Agotado!`
                        visible: inStock <= 6
                        font.pointSize: 14
                    }
                    Button {
                        text: "Actualizar"
                        visible: userIsAdmin
                        onClicked: updateInStock.open()
                    }
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

                Review {
                    id: review
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: root.height*.4
                    productId: root.productId
                }

                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: root.height*.4
                    Layout.topMargin: 20
                    model: reviewsModel
                    delegate: ReviewDelegate {}
                    clip: true
                    spacing: 20
                }
            }
        }
    }

    StockPopup {
        id: updateInStock
        onOkClicked: if (quantity.text) api.patchStock(quantityField.text)
    }

    ProductApi { id: api }

    ListModel { id: reviewsModel }

    ReviewApi {
        id: reviewApi
        onAllReviewsFetched: r => {
            const customerReview = r.find(r => r.customer_id === customerId);
            if (customerReview)
                review.fillReview(customerReview);
            reviewsModel.append(
                r.filter(item => item.customer_id !== customerId)
            );
        }
        Component.onCompleted: fetchAllReviews(productId)
    }
}
