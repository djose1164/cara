import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Layouts
import Billing
import Utils

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    title: "Configuraciones"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        BillingSettingsItem {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
        }
    }
}
