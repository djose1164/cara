import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Shopping
import Cara.Controls

Page {
    id: root

    readonly property T.StackView myStack: StackView.view
    readonly property alias categoryId: categoryCB.currentValue
    readonly property alias description: descriptionField.text
    readonly property alias name: nameField.text
    property var model
    property Component buttonComponent
    property alias actionButtonComponent: actionButtonLoader.sourceComponent
    property alias pricingLoader: pricingLoader
    property alias imgPicker: imgPicker
    readonly property alias supplierLoader: supplierLoader
    readonly property alias supplierItem: supplierLoader.item
    readonly property alias popup: popup
    readonly property alias pricingItem: pricingLoader.item
    readonly property alias nameField: nameField
    readonly property alias descriptionField: descriptionField
    readonly property alias categoryCB: categoryCB
    readonly property alias same: internal.same

    signal pictureAccepted(selectedPicture: url)
    signal deletePictureClicked(index: int)
    signal refreshRequested(productId: int)

    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: root.buttonComponent
        onBackClicked: root.myStack.pop()
    }

    Component.onCompleted: {
        internal.setOldProperties();
    }

    component SupplierInfoBox: GroupBox {
        title: "Suplidor"
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: `ID: <b>${root.model.supplierId}</b>`
                Layout.fillWidth: true
            }

            Label {
                text: `Nombre: <b>${root.model.supplierName}</b>`
                Layout.fillWidth: true
            }

            Label {
                text: `Precio: <b>RD$${root.model.supplierPrice}</b>`
                Layout.fillWidth: true
            }

            Button {
                text: "Actualizar"
                Layout.fillWidth: true
                onClicked: {
                    root.myStack.pushItem(
                        Qt.resolvedUrl("UpdateSupplierView.qml"),
                        {model: root.model}
                    );
                    const currentItem = root.myStack.currentItem;
                    currentItem.refreshRequested.connect(productId => {
                        console.log("Emittig refreshRequested from ProductForm. productId: "+productId);
                        root.refreshRequested(productId);
                    });
                }
            }
        }
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
                spacing: 20

                ProductItem {
                    id: productItem
                    images: root.model.images
                    deleteBtn.visible: root.model.images.length > 1
                    favoriteBtn.visible: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onDeletePictureClicked: index => root.deletePictureClicked(index)

                    Binding on images {
                        when: root.model.images.length === 0
                        value: [{imageUrl: internal.picturePlaceHolder}]
                    }
                }

                MenuSeparator { Layout.alignment: Qt.AlignCenter }

                TextField {
                    visible: root.model.productId > 0
                    enabled: false
                    placeholderText: "Product ID"
                    text: root.model.productId
                    Layout.fillWidth: true
                }

                TextField {
                    id: nameField
                    text: root.model.productName
                    placeholderText: "Nombre del producto"
                    Layout.fillWidth: true
                }

                TextField {
                    id: descriptionField
                    text: root.model.description
                    placeholderText: "Descripcion del producto"
                    Layout.fillWidth: true
                }

                Loader {
                    id: supplierLoader
                    sourceComponent: SupplierInfoBox {}
                    Layout.fillWidth: true
                }

                Loader {
                    id: pricingLoader
                    Layout.fillWidth: true
                }

                CategoryComboBox {
                    id: categoryCB
                    Layout.preferredWidth: root.width/2
                    currentIndex: root.model.categoryId - 1
                }

                Loader {
                    id: actionButtonLoader
                    Layout.fillWidth: true
                }

                Item {}
            }
        }
    }

    FileDialog {
        id: imgPicker
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: root.pictureAccepted(imgPicker.selectedFile)
    }

    StatusPopup {
        id: popup
        width: root.width*.8
        height: root.height/2
    }

    QtObject {
        id: internal
        readonly property url picturePlaceHolder: "cara-logo-v1.png"
        readonly property bool same: root.name === internal.oldName
                                     && root.description === internal.oldDescription
                                     && root.categoryId === internal.oldCategoryId
        property int pictureId: -1
        property string oldName
        property string oldDescription
        property int oldPrice
        property int oldCategoryId

        function setOldProperties() {
            internal.oldName = root.name;
            internal.oldDescription = root.description;
            internal.oldCategoryId = root.model.categoryId;
        }
    }

    function toJson() {
        const formJson = {
            name: nameField.text,
            description: descriptionField.text,
            categoryId: categoryCB.currentValue,
        };

        const productImages = model.images;
        if (productImages[0].imageUrl.toString() === internal.picturePlaceHolder.toString())
            return formJson;

        if (productImages.length > 0)
            formJson.images = model.images;
        return formJson;
    }

    function _handlePictureSelected(filename: url) {
        if (filename.toString() === "") {
            console.log("handlePictureSelected:", filename);
            throw "handlePictureSelected: " + filename;
        }
    }

    function _handlePictureDeleted(index: int) {
        try {
            root.model.images.splice(index, 1);

        } catch (err) {
            console.log(err);
            popup.state = "error";
        }
    }

    function errorHandler(err) {
        console.log(err);
        popup.text = err;
        popup.state = "error";
    }

    function openBusyPopup() {
        popup.state = "busy";
        popup.open();
    }

    function closeBusyPopup() {
        popup.state = "";
        popup.close();
    }
}
