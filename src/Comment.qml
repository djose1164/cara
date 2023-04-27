import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: root
    property alias text: reviewLbl.text
    property alias textField: commentEntry.commentText

    signal sendClicked
    signal cancelClicked

    onCurrentIndexChanged: {
        if (currentIndex === 0)
            commentEntry.commentText = root.text;
    }

    width: 200

    CommentEntry { id: commentEntry }

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

