import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"
import "qrc:/reviews"

ColumnLayout {
    id: root

    required property int productId
    readonly property var review: {
        "review": comment.textField,
        "rating": starRating.selectedStars,
        "customer_id": TheCurrentUser.customerId,
        "product_id": productId
    }
    readonly property int customerId: TheCurrentUser.customerId
    readonly property string name: `${TheCurrentUser.forename} ${TheCurrentUser.surname}`
    readonly property bool isReviewOwner: reviewCustomerId === customerId
    property int reviewId
    property bool isCommentable

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
        let options = { timeZone: "America/Santo_Domingo" };
        let jsDate = new Date(formattedDate);
        let currentDate = new Date();
        print(`formattedDate: ${formattedDate}\njsDate: ${jsDate.toLocaleString()}\ncurrentDate: ${currentDate.toLocaleString()}`)
        let msec = currentDate - jsDate;
        print(`msec: ${msec}`);
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

    ReviewHeader {
        id: reviewHeader
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredWidth: isReviewOwner ? root.width : root.width/2
        Layout.maximumWidth: Constants.appWidth*1.5
        customerNameTxt: customerName
        dateTxt: agoDate
        canModify: isReviewOwner
        onEditClicked: comment.currentIndex = 0
        onDeleteClicked: api.deleteReview(reviewId)
    }

    StarRating {
        id: starRating
        Layout.fillWidth: true
        Layout.fillHeight: true
        selectedStars: reviewRating
        enabled: isReviewOwner
    }

    Comment {
        id: comment
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.topMargin: 20
        Layout.bottomMargin: 20
        onSendClicked: {
            if (!starRating.selectedStars) {
                openDialogWithText("Debes seleccionar al menos 1 estrella.");
                return;
            }
            else if (TheCurrentUser.isGuest){
               openDialogWithText("Debes estar registrado para dejar tu reseña.");
               return;
           }
            api.postReview(review);
        }
        currentIndex: 1
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
            comment.text = review.review;
            comment.currentIndex = 1;
            openDialogWithText("¡Reseña posteada con exito!");
            reviewId = r.rating_id;
            agoDate = "Justo ahora";
        }
        onReviewFetched: rating => root.fillReview(rating)
        onReviewDeleted: {
            starRating.selectedStars = 0;
            comment.currentIndex = 0;
            reviewId = 0;
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
        setDate(rating.posted_date);
    }

    function setCommentText(text: string) {
        comment.text = text;
        comment.currentIndex = 1;
    }

    function changeTimeZone(date, timeZone) {
        if (typeof date === 'string') {
            return new Date(
                new Date(date).toLocaleString('en-US', {
                    timeZone,
                }),
            );
        }

        return new Date(date.toLocaleString('en-US', {
                timeZone,
            }),
        );
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
