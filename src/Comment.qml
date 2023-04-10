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

    Layout.fillWidth: true
    Layout.minimumHeight: root.height*.2

    ColumnLayout {
        ScrollView {
            Layout.fillWidth: true
            Layout.minimumHeight: root.height*.2

            TextArea {
                id: textArea
                placeholderText: "Escriba su oponion..."
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            visible: textArea.focus

            spacing: 20

            Button {
                onClicked: {
                    textArea.focus = false;
                    if (commentMenu.hasText)
                        gotoCommentView();

                }
                text: "Cancelar"
            }

            Button {
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

