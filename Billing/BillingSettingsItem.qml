import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: root
    
    implicitWidth: label.implicitWidth + spinBox.implicitWidth
    implicitHeight: label.implicitHeight + spinBox.implicitHeight

    Label {
        id: label
        width: root.width * .7
        anchors.top: parent.top
        anchors.left: parent.left
        text: "Limite maximo de facturas en PDF a almacenar"
        wrapMode: Label.Wrap
    }

    SpinBox {
        id: spinBox
        width: 100
        height: 35
        anchors.top: parent.top
        anchors.right: parent.right
        from: 1
        to: 256
        value: BillingSettings.maximumSavedPdfFiles
    }

    Binding {
        target: BillingSettings
        property: "maximumSavedPdfFiles"
        value: spinBox.value
    }
}
