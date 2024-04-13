import QtQuick
import QtQuick.Controls

TextField {
    validator: RegularExpressionValidator {
        regularExpression: /[a-z0-9]+@[a-z]+\.[a-z]{2,3}/
    }
    placeholderText: "Correo electrónico"
    inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
}
