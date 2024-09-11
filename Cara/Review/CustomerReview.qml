import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls
import Cara.User
import Cara.Review

Item {
    id: root

    property date createdAt
    property string reviewer
    property int rating
    property string reviewText
    property int customerId
    property alias canDelete: reviewHeader.canDelete
    property int reviewId

    property string agoDate: {
        const dateString = root.createdAt.toISOString();
        console.log(dateString)
        let formattedDate = dateString.replace( /[-]/g, '/' );
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

    signal reviewDeleted

    implicitHeight: columnLayout.implicitHeight
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.margins: spacing

        ReviewHeader {
            id: reviewHeader
            customerNameTxt: root.reviewer
            dateTxt: agoDate
            Layout.preferredWidth: root.width/2
            onDeleteClicked: ReviewController.deleteReview(root.reviewId)
        }

        StarRating {
            id: stars
            starSize: "16x16"
            selectedStars: root.rating - 1
            Layout.leftMargin: 0
        }

        Label {
            id: textLbl
            text: root.reviewText
            wrapMode: Label.Wrap
            Layout.fillWidth: true
        }
    }

    Connections {
        target: ReviewController

        function onReviewDeleted() {
            root.reviewDeleted();
        }
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onAccepted: close()
        onButtonClicked: close()
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

    function subtractHours(date: date, hours: int) {
        date.setHours(date.getHours() - hours);
        return date;
    }
}
