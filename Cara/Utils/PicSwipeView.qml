import QtQuick
import QtQuick.Controls
import Cara.Network

SwipeView {
    id: root

    property alias model: repeater.model

    Repeater {
        id: repeater
        Loader {
            id: loader
            required property string imageUrl
            active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
            sourceComponent: Image {
                source: {
                    if (loader.imageUrl.startsWith("http")
                            || loader.imageUrl.startsWith("file://"))
                        return loader.imageUrl;
                    else
                        return Api.uploadsPath + loader.imageUrl;
                }
                sourceSize: internal.sourceSize
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
