import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    readonly property var address: {
        "country": country.text,
        "province": province.text,
        "municipality": municipality.text,
    }

    spacing: 20

    TextField {
        id: country
        placeholderText: "Pais"
        text: "Republica Dominicana"
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    TextField {
        id: province
        placeholderText: "Provincia"
        text: "Monseñor Nouel"
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    TextField {
        id: municipality
        placeholderText: "Municipio"
        text: "Bonao"
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    TextField {
        id: street
        placeholderText: "Calle"
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }

    TextField {
        id: house_num
        placeholderText: "Numero de casa"
        Layout.columnSpan: 2
        Layout.fillWidth: true
    }
}
