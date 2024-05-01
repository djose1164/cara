pragma Singleton
import QtQuick
import QtCore

Settings {
    id: root

    property int maximumSavedPdfFiles

    category: "Billing"

    Component.onCompleted: if (maximumSavedPdfFiles === 0) maximumSavedPdfFiles = 5
}
