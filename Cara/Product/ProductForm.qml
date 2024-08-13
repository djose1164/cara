import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Utils
import Cara.Shopping
import Cara.Inventory

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
    readonly property alias popup: popup
    readonly property alias pricingItem: pricingLoader.item
    readonly property alias nameField: nameField
    readonly property alias descriptionField: descriptionField
    readonly property alias categoryCB: categoryCB
    readonly property alias same: internal.same


    signal pictureAccepted(selectedPicture: url)
    signal deletePictureClicked(index: int)

    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: root.buttonComponent
        onBackClicked: myStack.pop()
    }

    Component.onCompleted: {
        internal.setOldProperties();
        //console.log("ProductForm: "+model.categoryId);
        console.log("Productname:",root.model.productName)
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
                spacing: 10

                ProductItem {
                    id: productItem
                    images: model.images
                    deleteBtn.visible: model.images.length > 1
                    favoriteBtn.visible: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onDeletePictureClicked: index => root.deletePictureClicked(index)

                    Binding on images {
                        when: model.images.length === 0
                        value: [{imageUrl: internal.picturePlaceHolder}]
                    }
                }

                MenuSeparator { Layout.alignment: Qt.AlignCenter }

                FileDialog {
                    id: imgPicker
                    currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                    onAccepted: root.pictureAccepted(imgPicker.selectedFile)
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
            }
        }
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
            category_id: categoryCB.currentValue,
        };
        if (model.images.length > 0)
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
