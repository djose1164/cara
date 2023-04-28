import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    readonly property StackLayout stackLayout: StackLayout.layout
    property alias commentText: textArea.text
    property alias textArea: textArea

    width: parent.width
    height: childrenRect.height

    ScrollView {
        id: scrollView
        width: parent.width
        Layout.columnSpan: 2
        TextArea {
            id: textArea
            focus: true
            placeholderText: "Escriba su opinion..."
        }
    }

    RowLayout {
        anchors.top: scrollView.bottom
        anchors.right: parent.right
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
