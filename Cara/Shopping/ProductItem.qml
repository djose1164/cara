import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    readonly property alias favoriteBtn: likeButton
    readonly property alias deleteBtn: deleteBtn
    readonly property alias loader: loader
    property string name
    property alias images: imageView.model

    signal favoriteClicked(favorite: bool)
    signal deletePictureClicked(index: int)

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.topMargin: spacing
        spacing: 20

        Label {
            id: label
            text: root.name
            font.pointSize: 14
            horizontalAlignment: Label.AlignHCenter
            Layout.fillWidth: true
        }

        PicSwipeView {
            id: imageView
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
        }

        Item {
            Layout.fillWidth: true
            implicitHeight: Math.max(likeButton.implicitHeight, deleteBtn.implicitHeight)
            LikeButton {
                id: likeButton
                onClicked: like => root.favoriteClicked(like)
            }

            PageIndicator {
                id: pageIndicator
                anchors.centerIn: parent
                currentIndex: imageView.currentIndex
                count: imageView.count
            }

            RoundButton {
                id: deleteBtn
                anchors.right: parent.right
                flat: true
                icon.source: "qrc:/icons/trash-can-outline.svg"
                onClicked: root.deletePictureClicked(imageView.currentIndex)
            }
        }

        Loader {
            id: loader
            visible: false
        }
    }

    QtObject {
        id: internal
        readonly property int bestSize: Math.min(root.width/2, root.height/2)
        readonly property string sourceSize: "150x150"
    }
}
