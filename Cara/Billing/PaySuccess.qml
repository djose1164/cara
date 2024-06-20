import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// TODO: Make two views: one for just seeing & the another for editing.
ColumnLayout {
    Label {
        text: "¡Cambios realizados sastifactoriamente!"
        Layout.alignment: Qt.AlignCenter
    }

    Image {
        source: "qrc:/icons/done.svg"
        sourceSize: "48x48"
        Layout.alignment: Qt.AlignCenter
    }
}
