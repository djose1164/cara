import QtQuick
import QtQuick.Controls
import QtMultimedia

Item {
    CaptureSession {
        camera: Camera {
            id: camera

            focusMode: Camera.FocusModeAutoNear
            customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
        }
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

}
