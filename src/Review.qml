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
    property int reviewId
    property alias dateStr: dateLbl.text

    Component.onCompleted: api.fetchReview(customerId, productId)

    spacing: 20

    Label {
        text: "Reseñas"
        font.bold: true
        font.pointSize: 16
    }

    RowLayout {
        Label {
            text: name
            font.pointSize: 12
            font.bold: true
        }
        Label {
            id: dateLbl
            Layout.alignment: Qt.AlignRight
            font.pointSize: 10
        }
    }
    StarRating {
        id: starRating
        Layout.fillWidth: true
        Layout.minimumHeight: root.height*.2
    }

    Comment {
        id: comment
        Layout.fillWidth: true
        onEditClicked: stackLayout.currentIndex = 0
        onDeleteClicked: api.deleteReview()
        onSendClicked: {
            if (!starRating.selectedStars) {
                openDialogWithText("Debes seleccionar al menos 1 estrella.");
                return;
            }
            api.postReview(review);
        }
    }

    MessageDialog { id: message }

    ReviewApi {
        id: api
        onReviewPosted: r => {
            comment.text = review.review;
            comment.currentIndex = 1;
            openDialogWithText("¡Reseña posteada con exito!");
            reviewId = r.rating_id;
            dateStr = "Justo ahora";
        }
        onReviewFetched: rating => root.fillReview(rating)
        onReviewDeleted: {
            starRating.starClicked(-1)
            comment.currentIndex = 0;
            reviewId = 0;
            dateStr = "";
        }
    }

    function openDialogWithText(text: string) {
        message.text = text;
        message.open();
    }

    function fillReview(rating: var) {
        starRating.starClicked(rating.rating-1);
        comment.text = rating.review;
        comment.currentIndex = 1;
        reviewId = rating.id;
        setDate(rating.posted_date);
    }

    function setDate(date: string) {
        let formattedDate = date.replace( /[-]/g, '/' );
        // parse the proper date string from the formatted string.
        formattedDate = Date.parse( formattedDate );
        const options = {weekday: ''};
        let jsDate = new Date(formattedDate);

        let currentDate = new Date();
        let msec = currentDate - jsDate;
        if (Math.floor(msec/1000) <= 60)
            dateLbl.text = `Hace ${Math.floor(msec/1000)} segundos`;
        else if (Math.floor(msec/1000/60) <= 60)
            dateLbl.text = `Hace ${Math.floor(msec/1000/60)} minutos`;
        else if (Math.floor(msec/1000/60/60) <= 60) {
            const hours = Math.floor(msec/1000/60/60);
            const hourStr = hours === 1 ? 'hora' : 'horas';
            dateLbl.text = `Hace ${hours} ${hourStr}`;
        }
        else if (Math.floor(msec/1000/60/60/24) <= 60) {
            const days = Math.floor(msec/1000/60/60/24);
            const dayStr = days === 1 ? "dia" : "dias";
            dateLbl.text = `Hace ${days} ${dayStr}`;
        }
        else if (Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth()) <= 12) {
            const months = Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth());
            const monthStr = months === 1 ? "mes" : "meses";
            dateLbl.text = `Hace ${months} ${monthStr}`;
        }
        else {
            const years = Math.floor(msec/1000/60/60/24/getDaysInCurrentMonth()/12);
            const yearStr = years === 1 ? "año" : "años";
            dateLbl.text = `Hace ${years} ${yearStr}`;
        }
    }

    function getDaysInCurrentMonth() {
      const date = new Date();
      return new Date(date.getFullYear(), date.getMonth() + 1, 0).getDate();
    }
}
