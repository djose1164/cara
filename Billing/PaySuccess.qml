import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import entities
import Utils
import Billing

// TODO: Make two views: one for just seeing & the another for editing.
ColumnLayout {
    Label {
        text: "¡Cambios realizados sastifactoriamente!"
        Layout.alignment: Qt.AlignCenter
    }

    IconImage {
        source: "qrc:/icons/done.svg"
        sourceSize: "48x48"
        Layout.alignment: Qt.AlignCenter
    }
}
