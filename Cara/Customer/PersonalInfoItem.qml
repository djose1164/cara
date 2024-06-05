import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls
import Cara.Utils
import "qrc:/qt/qml/Cara/Utils/utils.js" as JS

Item {
    id: root

    default property alias contentData: columnLayout.data
    property int sideMargin: 15
    property alias forename: forename.text
    property alias surname: surname.text
    property alias birthday: dateField.currentDate
    property alias columnLayout: columnLayout

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.leftMargin: root.sideMargin
        anchors.rightMargin: root.sideMargin
        spacing: 20

        TextField {
            id: forename
            placeholderText: "Nombre"
            Layout.fillWidth: true
        }

        TextField {
            id: surname
            placeholderText: "Apellido"
            Layout.fillWidth: true
        }

        DateField {
            id: dateField
            validator: DateValidator {
                from: {
                    const now = new Date();
                    now.setFullYear(now.getFullYear() - 100);
                    return now;
                }

                to: new Date()
            }
            Layout.fillWidth: true
        }
    }

    function validate(): void {
        console.log("dateField.length:",dateField.length)
        if (JS.isEmpty(forename.text))
            throw new Error("El nombre no puede estar vacio.");
        else if (JS.isEmpty(surname.text))
            throw new Error("El apellido no puede estar vacio.");
        else if (!dateField.empty && !dateField.acceptableInput)
            throw new Error("Fecha de nacimiento invalida.")
    }

    function clear(): void {
        forename.clear();
        surname.clear();
    }

    function toJson() {
        return {
            forename: root.forename,
            surname: root.surname,
            birthday: dateField.displayDate.toISOString()
        };
    }

    function fillFields(data) {
        if (data?.forename)
            root.forename = data.forename;
        if (data?.surname)
            root.surname = data.surname;
        if (data?.birthday)
            root.birthday = data.birthday;
    }
}
