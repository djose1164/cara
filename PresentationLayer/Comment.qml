import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: root
    property alias text: reviewLbl.text
    property alias textField: commentEntry.commentText
    readonly property bool isEditing: currentIndex === 0
    readonly property alias focusInTextArea: commentEntry.focusInTextArea

    signal sendClicked
    signal cancelClicked

    onCurrentIndexChanged: {
        if (currentIndex === 0)
            commentEntry.commentText = root.text;
    }

    height: isEditing ? 100 : 40

    CommentEntry { id: commentEntry }

    Label {
        id: reviewLbl
        font.pointSize: 12
        wrapMode: Label.Wrap
        textFormat: Label.MarkdownText
    }

    function gotoCommentView() {
        root.currentIndex = 1;
    }

    function gotoCommentEdit(withFocus = false) {
        commentEntry.setFocusInTextArea(withFocus);
        root.currentIndex = 0;
    }
}

