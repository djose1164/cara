import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/singleton"
import "qrc:/reviews"
import core

ColumnLayout {
    id: root

    required property int productId
    readonly property var review: {
        "review": comment.textField,
        "rating": starRating.selectedStars,
        "customer_id": TheCurrentUser.customerId,
        "product_id": productId
    }
    readonly property var reviewToPatch: {
        "review": comment.textField,
        "rating": starRating.selectedStars
    }

    readonly property int customerId: TheCurrentUser.customerId
    readonly property string name: `${TheCurrentUser.forename} ${TheCurrentUser.surname}`
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

    onReviewTextChanged: {
        comment.text = reviewText;
    }

    width: parent.width
    height: {
        if (comment.isEditing)
            if (comment.focusInTextArea)
                return 150;
            else
                return 120;
        else if (reviewText)
            return 140;
        else
            return 80;
    }
    spacing: 10

    ReviewHeader {
        id: reviewHeader
        Layout.preferredWidth: isReviewOwner ? root.width : root.width/2
        Layout.maximumWidth: Constants.appWidth*1.5
        customerNameTxt: customerName
        dateTxt: agoDate
        canModify: isReviewOwner && reviewText
        onEditClicked: {
            comment.gotoCommentEdit(true);
            selectedStarsBeforePatch = starRating.selectedStars;
        }
        onDeleteClicked: api.deleteReview(reviewId)
    }

    StarRating {
        id: starRating
        selectedStars: reviewRating
        enabled: isReviewOwner && comment.isEditing
        visible: !comment.isEditing
    }

    Comment {
        id: comment
        Layout.fillWidth: true
        Layout.fillHeight: true
        onCancelClicked: {
            starRating.selectedStars = selectedStarsBeforePatch;
            reviewHeader.isEditing = false;
        }
        onSendClicked: {
            if (!starRating.selectedStars) {
                openDialogWithText("Debes seleccionar al menos 1 estrella.");
                return;
            }
            else if (TheCurrentUser.isGuest){
               openDialogWithText("Debes estar registrado para dejar tu reseña.");
               return;
           }
            if (reviewHeader.isEditing) {
                api.patchReview(reviewId, reviewToPatch);
                reviewHeader.isEditing = false;
            }
            else
                api.postReview(review);
        }
        currentIndex: reviewText ? 1
                                 : isReviewOwner ? 0 : 1
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onAccepted: close()
        onButtonClicked: close()
    }

    ReviewApi {
        id: api
        onReviewPosted: r => {
            reviewText = review.review;
            comment.currentIndex = 1;
            openDialogWithText("¡Reseña posteada con exito!");
            reviewId = r.rating_id;
            agoDate = "Justo ahora";
            customerName = name;
        }
        onReviewPatched: {
            reviewHeader.isEditing = false;
            comment.gotoCommentView();
            reviewText = comment.textField;
        }
        onReviewFetched: rating => root.fillReview(rating)
        onReviewDeleted: {
            starRating.selectedStars = 0;
            comment.gotoCommentEdit(false);
            reviewId = 0;
            comment.textField = reviewText = agoDate = "";
        }
    }

    function openDialogWithText(text: string) {
        message.text = text;
        message.open();
    }

    function fillReview(rating: var) {
        starRating.selectedStars = rating.rating;
        reviewId = rating.id;
        customerName = rating.name;
        setCommentText(rating.review);
    }

    function setCommentText(text: string) {
        comment.text = text;
    }

    function getDaysInCurrentMonth() {
      const date = new Date();
      return new Date(date.getFullYear(), date.getMonth() + 1, 0).getDate();
    }

    function subtractHours(date: Date, hours: int) {
        date.setHours(date.getHours() - hours);
        return date;
    }
}
