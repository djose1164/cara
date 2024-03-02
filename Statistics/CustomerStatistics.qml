import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Customer
import Utils

Page {
    id: root

    required property int salespersonId
    readonly property T.StackView stackView: StackView.view

    title: "Estadisticas de Clientes"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    Component.onCompleted: {
        customersView.updateModel(salespersonId);
    }

    CustomerViewItem {
        id: customersView
        anchors.fill: parent
        anchors.topMargin: 5
        onCustomerSelected: customer => stackView.push("CustomerStatisticsDetails.qml", {customer})
    }
}
