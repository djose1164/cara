import QtQuick
import QtQuick.Controls

TextField {
    id: textField
    property bool showText: false

    placeholderText: "Contraseña"
    echoMode: showText ? TextField.Normal : TextField.Password
    passwordMaskDelay: 600
    rightPadding: eyeIcon.width + font.pointSize
    inputMethodHints: Qt.ImhSensitiveData | Qt.ImhNoPredictiveText
    renderType: Text.NativeRendering
    cursorDelegate: Rectangle {
        visible: false
        width: 1
        color: "red"
    }

    EnterKey.type: Qt.EnterKeyDone
    onEditingFinished: {
        focus = false
        Qt.inputMethod.hide()
    }


    Image {
        id: eyeIcon
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        antialiasing: false
        source: parent.showText ? "qrc:/icons/eye-crossed.svg" : "qrc:/icons/eye.svg"
        sourceSize.width: 25
        sourceSize.height: 25

        TapHandler {
            onTapped: showText = !showText
        }
    }
}
