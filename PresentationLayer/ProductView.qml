import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: stackLayout

    property alias delegate: listView.delegate
    property alias spacing: listView.spacing
    property alias listView: listView

    Component.onCompleted: productRequest.addSuggetionsToModel()

    BusyIndicator {}

    ListView {
        id: listView

        readonly property StackView stackView: root.stackView

        spacing: 20
        model: productRequest.model
    }

    ProductRequest {
        id: productRequest
        onSucceeded: stackLayout.currentIndex = 1
    }
}

