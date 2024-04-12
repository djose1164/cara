import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Utils
import Review

Drawer {
    id: root

    property alias selectedStars: stars.selectedStars
    property alias reviewText: reviewArea.text
    readonly property alias starsCount: stars.starsCount

    readonly property bool same: internal.oldReviewText === root.reviewText
                                 && internal.oldSelectedStars === stars.starsCount
    readonly property bool empty: stars.starsCount === -1 && reviewArea.text === ""

    signal postClicked(newReview: review)
    signal updateClicked

    onOpened: root.setOldProperties()
    Component.onCompleted: {
        root.setOldProperties();
    }

    Page {
        anchors.fill: parent
        header: RowLayout {
            CloseButton {
                onClicked: {
                    if (!root.same && !root.empty)
                        message.open();
                    else
                        root.close();
                }

                icon.color: "black"
            }

            Label {
                text: "Redacta tu reseña"
                elide: Label.ElideRight
                font.pointSize: 18
                font.bold: true
                Layout.fillWidth: true
            }

            Button {
                flat: true
                text: "Publicar"
                enabled: stars.starsCount > 0 && !root.same
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 5
                onClicked: root.postClicked(root.toReview())
            }
        }

        ScrollView {
            anchors.fill: parent
            contentWidth: availableWidth

            ColumnLayout {
                id: columnLayout
                width: root.width * .8
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 40

                StarRating {
                    id: stars
                    Layout.fillWidth: true
                    Layout.topMargin: 40
                }

                Column {
                    id: column
                    TextField {
                        id: reviewArea
                        width: columnLayout.width
                        placeholderText: "¿Qué piensas sobre el producto?"
                        text: root.reviewText
                        wrapMode: TextArea.Wrap
                        maximumLength: internal.maximumWords
                    }

                    Label {
                        anchors.right: reviewArea.right
                        text: `${reviewArea.length}/${internal.maximumWords}`
                    }
                }
            }
        }
    }

    MessageDialog {
        id: message
        text: "¿Estas seguro de descartar tu reseña?"
        buttons: MessageDialog.Discard | MessageDialog.Cancel
        onButtonClicked: (button, role) => {
            switch (button) {
            case MessageDialog.Discard:
                root.clear();
                root.close();
                break;
            case MessageDialog.Cancel:
                break;
            }
        }
    }

    QtObject {
        id: internal
        readonly property int maximumWords: 512
        property string oldReviewText
        property int oldSelectedStars
    }

    function toReview(): review {
        return {
            rating: stars.starsCount + 1,
            reviewText: reviewArea.text,
        };
    }

    function setOldProperties() {
        internal.oldSelectedStars = root.selectedStars;
        internal.oldReviewText = reviewArea.text;
    }

    function clear() {
        root.selectedStars = 0;
        root.reviewText = "";
    }
}
