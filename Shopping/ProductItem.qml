import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    readonly property alias favoriteBtn: likeButton
    property var product

    signal favoriteClicked(favorite: bool)

    implicitWidth: columnLayout.implicitWidth
    implicitHeight: columnLayout.implicitHeight

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.topMargin: spacing
        spacing: 20

        Label {
            id: label
            text: product.name
            font.pointSize: 14
            horizontalAlignment: Label.AlignHCenter
            Layout.fillWidth: true
        }

        SwipeView {
            id: imageView
            clip: true
            Layout.fillWidth: true

            Repeater {
                model: product.images
                Loader {
                    required property string image_url
                    active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                    sourceComponent: Image {
                        source: image_url
                        sourceSize: internal.sourceSize
                        fillMode: Image.PreserveAspectFit
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }

        LikeButton {
            id: likeButton
            onClicked: like => root.favoriteClicked(like)
        }

        PageIndicator {
            id: pageIndicator
            currentIndex: imageView.currentIndex
            count: imageView.count
            Layout.alignment: Qt.AlignHCenter
        }

    }

    QtObject {
        id: internal
        readonly property int bestSize: Math.min(root.width/2, root.height/2)
        readonly property string sourceSize: "150x150"
    }
}
