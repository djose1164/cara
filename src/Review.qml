import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"

ColumnLayout {
    id: root

    required property int productId
    readonly property var review: {
        "review": textArea.text,
        "rating": starRating.selectedStars,
        "customer_id": TheCurrentUser.customerId,
        "product_id": productId
    }
    readonly property int customerId: TheCurrentUser.customerId

    Component.onCompleted: api.fillReview()

    spacing: 20

    Label {
        text: "Reseñas"
        font.bold: true
        font.pointSize: 16
    }

    StarRating {
        id: starRating
        Layout.fillWidth: true
        Layout.minimumHeight: root.height*.2
    }

    StackLayout {
        id: stackLayout
        Layout.fillWidth: true
        Layout.minimumHeight: root.height*.2

        ColumnLayout {
            ScrollView {
                Layout.fillWidth: true
                Layout.minimumHeight: root.height*.2

                TextArea {
                    id: textArea
                    placeholderText: "Escriba su oponion..."
                }
            }

            RowLayout {
                Layout.fillWidth: true

                spacing: 20

                Button {
                    onClicked: stackLayout.currentIndex = 1
                    text: "Cancelar"
                }

                Button {
                    text: "Enviar"
                    onClicked: api.postReview()
                }
            }
        }

        RowLayout {
            Label {
                id: reviewLbl
                font.pointSize: 14
            }
            RoundButton {
                id: menuBtn
                property bool isMenuOpened
                icon.source: "qrc:/icons/more_vert.svg"
                flat: true
                onClicked: if (!isMenuOpened) menu.open()

                Menu {
                    id: menu
                    onOpened: menuBtn.isMenuOpened = true
                    onClosed: menuBtn.isMenuOpened = false
                    y: parent.height
                    height: 100
                    rightMargin: 10

                    MenuItem {
                        onTriggered: stackLayout.currentIndex = 0
                        text: "Editar"
                    }
                    MenuItem {
                        text: "Eliminar"
                    }
                }
            }
        }
    }

    MessageDialog { id: message }

    CaraApi {
        id: api
        endpoint: "/rating"

        function postReview() {
            if (!starRating.selectedStars) {
                openDialogWithText("Debes seleccionar al menos 1 estrella.");
                return;
            }
            const success = () => openDialogWithText("¡Reseña posteada con exito!");
            api.succeeded.connect(success);

            const failed = () => openDialogWithText("Error al postear reseña.");
            api.failed.connect(failed);

            api.post("/", review);
        }

        function fillReview() {
            const success = ({ rating }) => {
                starRating.starClicked(rating.rating-1);
                reviewLbl.text = rating.review;
                stackLayout.currentIndex = 1;
            }
            api.succeeded.connect(success);

            api.get(`/?customer_id=${customerId}&product_id=${productId}`);
        }
    }

    function openDialogWithText(text: string) {
        message.text = text;
        message.open();
    }
}

