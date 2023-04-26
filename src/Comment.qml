import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: root
    property alias text: reviewLbl.text
    property alias textField: textArea.text

    signal sendClicked

    onCurrentIndexChanged: {
        if (currentIndex === 0)
            textArea.text = root.text;
    }

    Item {
        ScrollView {
            id: editScrollView
            width: parent.width
            height: 50

            TextArea {
                id: textArea
                placeholderText: "Escriba su oponion..."
            }
        }

        RowLayout {
            width: parent.width*.6
            height: 40
            anchors.top: editScrollView.bottom
            anchors.right: editScrollView.right
            visible: textArea.focus
            spacing: 10
            Button {
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    textArea.focus = false;
                    if (text.length)
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

    Label {
        id: reviewLbl
        font.pointSize: 11
        wrapMode: Label.Wrap
        textFormat: Label.MarkdownText
    }

    function gotoCommentView() {
        root.currentIndex = 1;
    }

    function gotoCommentEdit() {
        root.currentIndex = 0;
    }
}

