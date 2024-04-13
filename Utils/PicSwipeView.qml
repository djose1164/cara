import QtQuick
import QtQuick.Controls
import data

SwipeView {
    id: root

    property alias model: repeater.model

    Repeater {
        id: repeater
        Loader {
            id: loader
            required property string image_url
            active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
            sourceComponent: Image {
                source: {
                    if (loader.image_url.startsWith("http")
                            || loader.image_url.startsWith("file://"))
                        return loader.image_url;
                    else
                        return Constants.uploadsPath + loader.image_url;
                }
                sourceSize: internal.sourceSize
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
