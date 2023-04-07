import QtQuick

Image {
    id: root
    required source
    onStatusChanged: {
        if (root.status === Image.Ready)
            stackLayout.currentIndex = 1;
        else if (root.status === Image.Error)
            source = "qrc:/assets/cara-logo-v1.png";
    }
}
