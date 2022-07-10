import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 340
    height: 680
    visible: true
    title: "RapiK"
    Material.theme: Material.Dark
    Material.accent: Material.Light

    StackView {
        anchors.fill: parent
        initialItem: 0

        Login {
            id:login
        }
    }
    
}
