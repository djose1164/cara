import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    readonly property StackLayout stackLayout: StackLayout.layout
    property alias commentText: textArea.text

    ScrollView {
        Layout.fillWidth: true

        TextArea {
            id: textArea
            focus: true
            placeholderText: "Escriba su opinion..."
        }
    }

    Row {
        Layout.alignment: Qt.AlignRight
        Layout.maximumHeight: 20
        visible: textArea.focus
        spacing: 20

        Button {
            onClicked: {
                textArea.focus = false;
                if (commentText)
                   stackLayout.gotoCommentView();
                stackLayout.cancelClicked();
            }
            text: "Cancelar"
        }

        Button {
            text: "Enviar"
            onClicked: stackLayout.sendClicked()
        }
    }
}
