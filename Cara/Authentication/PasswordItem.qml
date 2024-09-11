import QtQuick
import QtQuick.Controls

Item {
    id: root

    readonly property string text: firstPassword.text
    readonly property bool acceptable: firstPassword.text === secondPassword.text
    property alias spacing: column.spacing

    implicitWidth: column.implicitWidth
    implicitHeight: column.implicitHeight

    Column {
        id: column
        anchors.fill: parent
        spacing: 30

        PasswordInput {
            id: firstPassword
            width: column.width
            placeholderText: "Nueva contraseña"
        }

        PasswordInput {
            id: secondPassword
            width: column.width
            placeholderText: "Confirmar contraseña"
            ToolTip {
                x: secondPassword.leftPadding
                y: secondPassword.height
                width: secondPassword.width * .8
                visible: !root.acceptable
                         && internal.firstLength > 3
                         && internal.secondLength > 3
                         && (internal.sameLength
                             || internal.secondLength > internal.firstLength)
                text: "Las contraseñas no coinciden"
            }
        }
    }

    QtObject {
        id: internal
        readonly property int firstLength: firstPassword.text.length
        readonly property int secondLength: secondPassword.text.length
        readonly property bool sameLength: firstLength === secondLength
    }
}
