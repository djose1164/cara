import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:users"
import "qrc:/utils"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView ? stackView.bar : null
    readonly property bool userIsAdmin: TheCurrentUser.isAdmin
    readonly property var editableProduct: {
        "name": [nameField.text, patchName],
        "sell_price": [parseInt(sellPriceField.text), patchSellPrice],
        "description": [descriptionField.text, patchDescription],
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

        Item {
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Image {
                    id: img
                    source: image_url
                    sourceSize: `${root.width*.9}x${root.height*.4}`
                    onStatusChanged: if (img.status === Image.Ready) stackLayout.currentIndex = 1
                }

                StarRating {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: root.height*.1
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

                            if (data !== product[key]) {
                                print(`--key: ${key} data: ${data}`)
                                callable(data);}
                        });
                    }
                    font.pointSize: 12
                }
            }
        }
    }

    Popup {
        id: updateInStock
        property alias stackLayout: popupStackLayout
        width: parent.width*.8
        height: parent.height/2
        anchors.centerIn: Overlay.overlay
        modal: true

        StackLayout {
            id: popupStackLayout
            anchors.fill: parent
            ColumnLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: 20
    property var sendableProduct
                Label {
                    Layout.fillWidth: true
                    text: `Cantidad disponible en inventario ${inStock}.`
                    wrapMode: Label.Wrap
                }

                TextField {
                    id: quantityField
                    Layout.fillWidth: true
                    placeholderText: "Nueva cantidad..."
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 20
                    Button {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true
                        text: "Cancelar"
                        onClicked: updateInStock.close()
                    }
                    Button {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillWidth: true
                        text: "Ok"
                        onClicked: {
                            if (quantityField.text)
                                patchStock(quantityField.text);
                        }
                    }
                }
            }
            Label {
                text: "Operacion exitosa"
                font.pointSize: 14
            }

            Label {
                text: "Operacion fallida"
                font.pointSize: 14
            }
        }
    }

    CaraApi {
        id: api
        endpoint: "/products"
        onFailed: () => popupStackLayout.currentIndex = 2
    }

    function patchStock(inStock: int): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.inStock = inStock;
        };

        patchProduct({"in_stock": inStock}, success);
    }

    function patchName(name: string): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.name = name;
        };

        patchProduct({"name": name}, success);
    }

    function patchSellPrice(sellPrice: int): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.sell_price = sellPrice;
        };

       patchProduct({"sell_price":sellPrice}, success);
    }

    function patchDescription(description: int): void {
        const success = () => {
            popupStackLayout.currentIndex = 1;
            root.description = description;
        };

       patchProduct({"description": description}, success);
    }

    function patchProduct(toPatch: var, onSuccess: var): void {
        if (onSuccess)
            api.succeeded.connect(onSuccess);
        api.patch(`/${productId}`, toPatch);
    }
}
