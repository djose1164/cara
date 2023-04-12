import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"
import "qrc:/reviews"

ColumnLayout {
    id: root

    required property string review
    required property int rating
    required property string name
    required property string posted_date

    Component.onCompleted: {
        setDate(posted_date);
        starRating.selectedStars = rating;
    }

    width: ListView.view.width
    height: ListView.view.height/3

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
        Layout.fillHeight: true
        enabled: false
    }

    Label {
        Layout.fillWidth: true
        text: review
        font.pointSize: 11
        wrapMode: Label.Wrap
    }

    function setDate(date: string) {
        let formattedDate = date.replace( /[-]/g, '/' );
        // parse the proper date string from the formatted string.
        formattedDate = Date.parse( formattedDate );
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
