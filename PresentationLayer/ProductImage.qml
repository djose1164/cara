import QtQuick

Image {
    id: root
    required source

    signal ready

    onStatusChanged: {
        if (root.status === Image.Ready)
            root.ready();
        else if (root.status === Image.Error) {
            source = "qrc:/assets/cara-logo-v1.png";
            root.ready();
        }
    }
}
