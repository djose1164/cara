import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Component {
    ColumnLayout {
        width: billPage.width

        Label {
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.topMargin: 20
            text: name
            font.pointSize: 18
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            //Layout.leftMargin: 15

            Label {
                text: "Orden:"
                font.bold: true
                font.pointSize: 16
            }

            Repeater {
                model: orders

                Label {
                    id: leftLbl
                    text: "ID: " + id
                    font.pointSize: 16
                }
            }

            Repeater {
                model: orders

                Label {
                    text: "Condicion: " + (payment_status ? "Pago" : "No pago")
                    font.pointSize: 16
                }
            }
        }
    }
}
