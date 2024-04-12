import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import User
import Review

/**
  * The user will be able to write a review witha rating, edit it & delete it.
  */
ColumnLayout {
    id: root

    property int productId
    property review customerReview

    readonly property int customerId: UserSettings.customerId
    readonly property bool isReviewOwner: reviewCustomerId === customerId
    property int reviewId
    property bool isCommentable
    property int selectedStarsBeforePatch
    property string reviewText
    property int reviewRating
    property string customerName
    property string reviewPostedDate
    property int reviewCustomerId

    property string agoDate: {
        if (reviewPostedDate === "")
            return "";
        let formattedDate = reviewPostedDate.replace( /[-]/g, '/' );
        // parse the proper date string from the formatted string.
        formattedDate = Date.parse( formattedDate );

        let jsDate = new Date(formattedDate);
        let currentDate = new Date();
        let msec = currentDate - jsDate;

        if (Math.floor(msec/1000) <= 60)
            return `Hace ${Math.floor(msec/1000)} segundos`;
        else if (Math.floor(msec/1000/60) <= 60)
            return `Hace ${Math.floor(msec/1000/60)} minutos`;
        else if (Math.floor(msec/1000/60/60) <= 24) {
            const hours = Math.floor(msec/1000/60/60);
            const hourStr = hours === 1 ? 'hora' : 'horas';
            return `Hace ${hours} ${hourStr}`;
        }
        else if (Math.floor(msec/1000/60/60/24) <= 30) {
            const days = Math.floor(msec/1000/60/60/24);
            const dayStr = days === 1 ? "dia" : "dias";
            return `Hace ${days} ${dayStr}`;
        }
        else if (Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth()) <= 12) {
            const months = Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth());
            const monthStr = months === 1 ? "mes" : "meses";
            return `Hace ${months} ${monthStr}`;
        }
        else {
            const years = Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth()/12);
            const yearStr = years === 1 ? "año" : "años";
            return `Hace ${years} ${yearStr}`;
        }
    }

    states: [
        State {
            when: root.customerReview.reviewId > 0
            PropertyChanges {
                target: textLbl
                text: root.customerReview.reviewText
            }

            PropertyChanges {
                target: titileLbl
                text: `${UserSettings.forename} ${UserSettings.surname} (Tú)`
            }

            PropertyChanges {
                target: actionLbl
                text: "[Editar tu reseña](PostReview.qml)"
            }

            PropertyChanges {
                target: starRating
                enabled: false
                selectedStars: root.customerReview.rating - 1
            }

            PropertyChanges {
                target: moreVertBtn
                visible: true
            }

            PropertyChanges {
                target: postReview
                reviewText: root.customerReview.reviewText
                selectedStars: root.customerReview.rating - 1
            }
        },
        State {
            when: root.customerReview.reviewId === 0
            PropertyChanges {
                target: starRating
                selectedStars: -1
            }
        }
    ]

    implicitWidth: 300
    spacing: 10

    onCustomerReviewChanged: {
        console.log("review: "+JSON.stringify(root.customerReview, null, 2))
    }
    Component.onCompleted: {
        ReviewController.fetchReviewByCustomerIdAndProductId(root.customerId,
                                                             root.productId);
    }

    Item {
        Layout.fillWidth: true
        implicitHeight: Math.max(titileLbl.implicitHeight,
                                 moreVertBtn.implicitHeight)
        Label {
            id: titileLbl
            anchors.verticalCenter: parent.verticalCenter
            text: "Califica este producto"
            font.pointSize: 12
            font.bold: true
        }

        MoreVertButton {
            id: moreVertBtn
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            visible: false
            onClicked: menu.visible = !menu.visible

            Menu {
                id: menu
                y: moreVertBtn.height
                MenuItem {
                    text: "Eliminar"
                    onTriggered: ReviewController.deleteReview(root.customerReview.reviewId)
                }
            }
        }
    }

    StarRating {
        id: starRating
        onStarClicked: postReview.open();
    }

    Label {
        id: textLbl
        text: "Comparte con los demas tu reseña"
        wrapMode: Label.Wrap
        Layout.fillWidth: true
    }

    Label {
        id: actionLbl
        text: "[Redacta tu reseña](PostReview.qml)"
        textFormat: Label.MarkdownText
        onLinkActivated: () => postReview.open()
    }

    PostReview {
        id: postReview
        width: ApplicationWindow.window.width
        height: ApplicationWindow.window.height
        onReviewTextChanged: console.log(reviewText)
        onPostClicked: newReview => {
            if (root.customerReview.reviewId > 0) {
                newReview.reviewId = root.customerReview.reviewId;
                ReviewController.editReview(newReview);
            } else {
                newReview.customerId = UserSettings.customerId;
                newReview.productId = root.productId;
                ReviewController.postReview(newReview);

                openDialog("¡Gracias por tu reseña!");
            }
            postReview.close();
        }

        Binding on selectedStars {
            when: root.customerReview.reviewId === -1
            value: starRating.starsCount
        }
    }

    Connections {
        target: ReviewController

        function onReviewPosted(newReview: review) {
            root.customerReview = newReview;
        }

        function onReviewFetched(r: review) {
            root.customerReview = r;
        }

        function onErrorOccurred(error) {
            console.log("review error: "+error);
        }

        function onCustomerDisableToReview() {
            root.visible = false;
            console.log("onReviewDisabled");
        }

        function onReviewDeleted() {
            root.ToolTip.show("Reseña eliminada", internal.timeout);
            root.customerReview = root.createReview({reviewId: 0});
        }

        function onReviewEdited() {
            console.log("Reseña actualizada.");
            root.ToolTip.show("Reseña actualizada", internal.timeout);

            const newReview = root.customerReview;
            newReview.reviewText = postReview.reviewText;
            newReview.rating = postReview.starsCount + 1;
            root.customerReview = newReview;
        }
    }

    // ReviewHeader {
    //     id: reviewHeader
    //     Layout.preferredWidth: isReviewOwner ? root.width : root.width/2
    //     Layout.maximumWidth: app.width*1.5
    //     customerNameTxt: customerName
    //     dateTxt: agoDate
    //     canModify: isReviewOwner && reviewText
    //     onEditClicked: {
    //         comment.gotoCommentEdit(true);
    //         selectedStarsBeforePatch = starRating.selectedStars;
    //     }
    //     onDeleteClicked: api.deleteReview(reviewId)
    // }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onAccepted: close()
        onButtonClicked: close()
    }

    QtObject {
        id: internal
        readonly property int timeout: 2000
    }

    function openDialog(title: string, text: string) {
        message.title = title;
        message.text = text;
        message.open();
    }

    function getDaysInCurrentMonth() {
      const date = new Date();
      return new Date(date.getFullYear(), date.getMonth() + 1, 0).getDate();
    }

    function subtractHours(date: date, hours: int) {
        date.setHours(date.getHours() - hours);
        return date;
    }

    function createReview({
        reviewId = -1, reviewText = "", rating = 1, reviewer = ""
    }): review {
        return {
            reviewId,
            reviewText,
            rating,
            reviewer
        };
    }
}
