pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Cara.Network

SwipeView {
    id: root

    property size sourceSize: Qt.size(150, 150)
    property alias model: repeater.model

    Repeater {
        id: repeater
        Loader {
            id: loader
            required property string imageUrl
            active: SwipeView.isCurrentItem
                    || SwipeView.isNextItem
                    || SwipeView.isPreviousItem
            sourceComponent: Image {
                source: {
                    console.log(loader.imageUrl)
                    if (loader.imageUrl.startsWith("http")
                            || loader.imageUrl.startsWith("file://")
                            || loader.imageUrl.startsWith("content://"))
                        return loader.imageUrl;
                    else
                        return Api.uploadsPath + loader.imageUrl;
                }
                sourceSize: root.sourceSize
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
