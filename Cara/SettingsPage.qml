import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Cara.Billing
import Cara.Controls

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    title: "Configuraciones"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    Column {
        id: column
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 20
        topPadding: 10

        BillingSettingsItem {
            width: column.width
        }

        ItemDelegate {
            text: "Actualizar contraseña"
            icon.source: "qrc:/qt/qml/Cara/User/password.svg"
            width: column.width
            onClicked: root.stackView.pushItem("qrc:/qt/qml/Cara/User/UpdatePasswordPage.qml")
        }
    }
}
