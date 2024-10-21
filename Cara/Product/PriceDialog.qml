pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls

/*
  Used for setting a new price. This component has two preconditions:
    1- The old price cannot be equal to the entered price (newPrice property)
    2- The entered price (newPrice property) has to be greater than 0.
 */
Dialog {
    id: root

    default property alias content: columnLayout.data
    required property int oldPrice
    property var okButtonConditions: [() => root.newPrice > 0 && root.newPrice !== root.oldPrice]
    readonly property int newPrice: parseInt(newPriceField.text)

    implicitWidth: (ApplicationWindow.window?.width * .8) || 200
    implicitHeight: (ApplicationWindow.window?.height / 2) || 150
    anchors.centerIn: parent
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAboutToHide: newPriceField.clear()
    Component.onCompleted: {
        const okButton = root.standardButton(Dialog.Ok);
        okButton.enabled = Qt.binding(
            () => root.okButtonConditions.every(callable => callable())
        );
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        NumberTextField {
            id: newPriceField
            intValidator.bottom: 5
            placeholderText: "Nuevo precio RD$"
            ToolTip.text: "El nuevo precio no puede ser igual al actual."
            ToolTip.visible: parseInt(newPriceField.text) === root.oldPrice
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }
    }

    // Binding {
    //     target: root.okButton
    //     property: "enabled"
    //     value: root.newPrice > 0 && root.newPrice !== root.oldPrice
    // }
}
