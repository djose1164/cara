import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    readonly property StackLayout stackLayout: StackLayout.layout
    property alias commentText: textArea.text
    property alias textArea: textArea
    readonly property alias focusInTextArea: textArea.focus

    width: parent.width
    height: childrenRect.height

    ScrollView {
        id: scrollView
        width: parent.width
        Layout.columnSpan: 2
        TextArea {
            id: textArea
            placeholderText: "Escriba su opinion..."
        }
    }

    RowLayout {
        anchors.top: scrollView.bottom
        anchors.right: parent.right
        spacing: 20
        visible: textArea.focus

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

    function setFocusInTextArea(focus: bool) {
        textArea.focus = focus;
    }
}
