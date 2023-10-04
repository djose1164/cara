import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property string oldView

    signal viewChanged(newView: string)

    anchors.centerIn: Overlay.overlay
    Component.onCompleted: oldView = comboBox.currentText

    GridLayout {
        columns: 2

        Label {
            text: "Cambiar vista"
            font.pointSize: 14
            Layout.columnSpan: 2
        }

        Label {
            text: "Seleccionar vista"
        }

        ComboBox {
            id: comboBox
            model: ["Clientes", "Ordenes"]
        }

        Button {
            text: "Aceptar"
            onClicked: {
                const currentText = comboBox.currentText;
                if (oldView === currentText)
                    return;

                oldView = currentText;
                viewChanged(currentText);
            }
        }
    }
}
