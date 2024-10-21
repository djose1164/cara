import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Tiempo de espera agotado"
    standardButtons: Dialog.Retry | Dialog.Cancel

    Image {
        readonly property int bestSize: Math.min(root.width/2, root.height/2)

        anchors.centerIn: parent
        source: "qrc:/icons/warning.svg"
        sourceSize: `${bestSize}x${bestSize}`
    }
}
