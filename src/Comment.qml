import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: root
    property alias text: commentMenu.reviewText
    property alias textField: textArea.text

    signal editClicked
    signal deleteClicked
    signal sendClicked

    Item {
        ScrollView {
            id: editScrollView
            width: parent.width
            height: parent*.7

            TextArea {
                id: textArea
                placeholderText: "Escriba su oponion..."
            }
        }

        RowLayout {
            width: parent.width*.6
            height: parent*.3
            anchors.top: editScrollView.bottom
            anchors.right: editScrollView.right
            visible: textArea.focus
            spacing: 10
            Button {
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    textArea.focus = false;
                    if (commentMenu.hasText)
                        gotoCommentView();

                }
                text: "Cancelar"
            }

            Button {
                Layout.alignment: Qt.AlignRight
                text: "Enviar"
                onClicked: sendClicked()
            }
        }
    }

    CommentMenu {
        id: commentMenu
        onDeleteClicked: root.deleteClicked()
    }

    function gotoCommentView() {
        root.currentIndex = 1;
    }

    function gotoCommentEdit() {
        root.currentIndex = 0;
    }
}

