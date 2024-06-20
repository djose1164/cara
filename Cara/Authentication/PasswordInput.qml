import QtQuick
import QtQuick.Controls

TextField {
    id: textField

    property bool showText: false
    readonly property alias eyeBtn: eyeIcon

    placeholderText: "Contraseña"
    echoMode: showText ? TextField.Normal : TextField.Password
    passwordMaskDelay: 600
    rightPadding: eyeIcon.width
    inputMethodHints: Qt.ImhSensitiveData | Qt.ImhNoPredictiveText | Qt.ImhPreferNumbers
    EnterKey.type: Qt.EnterKeyDone
    hoverEnabled: true

    RoundButton {
        id: eyeIcon
        anchors.right: textField.right
        anchors.verticalCenter: textField.verticalCenter
        flat: true
        icon.source: showText ? "eye-crossed.svg" : "eye.svg"
        onClicked: if (textField.text.length) showText = !showText
    }
}
