import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property alias title: lbl.text
    property alias label: lbl
    property alias btnLoader: btnLoader
    property alias backBtn: leftArrowBtn

    signal backClicked

    RowLayout {
        id: row
        anchors.fill: parent

        BackButton {
            id: leftArrowBtn
            Layout.alignment: Qt.AlignLeft
            onClicked: backClicked()
        }

        Label {
            id: lbl
            font.bold: true
            font.pointSize: 15
            horizontalAlignment: btnLoader.status === Loader.Ready || !backBtn.visible ? Label.AlignHCenter
                                                                                       : Label.AlignLeft
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
        }

        Loader {
            id: btnLoader
        }
    }
}
