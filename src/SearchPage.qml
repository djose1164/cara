import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Buscador"
    header: RowLayout {
        width: parent.width

        ToolButton {
            background: Image {
                source: "qrc:/icons/left_arrow.svg"
                sourceSize: iconSize
            }
            onClicked: mainStackView.pop()
            Layout.topMargin: 5
            Layout.leftMargin: 2
        }

        Label {
            text: "Buscador"
            font.bold: true
            font.pointSize: 18
            Layout.alignment: Qt.AlignCenter
        }

        ToolButton {
            background: Image {
                source: "qrc:/icons/filter.svg"
                sourceSize: iconSize
            }
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 5
            Layout.rightMargin: 5
        }
    }

    ListView {
        id: searchListView
        anchors.fill: parent
        anchors.topMargin: 20
        headerPositioning: ListView.OverlayHeader
        header: Component {
            GridLayout {
                columns: 2
                columnSpacing: 0
                rowSpacing: 0
                width: parent.width
                height: 60
                TextField {
                    id: productField
                    placeholderText: "Nombre del producto"
                    Layout.fillWidth: true
                    Keys.onReturnPressed: addSuggetions(text, busyIndicator)
                    Layout.alignment: Qt.AlignTop
                    Layout.margins: 0
                    Layout.leftMargin: 10
                }

                ToolButton {
                    background: Image {
                        antialiasing: false
                        source: "qrc:/icons/search.svg"
                        sourceSize: iconSize
                    }
                    onClicked: addSuggetions(productField.text, busyIndicator)
                    Layout.alignment: Qt.AlignTop
                    Layout.margins: 0
                    Layout.rightMargin: 10
                }

                BusyIndicator {
                    id: busyIndicator
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    Layout.columnSpan: 2
                    Layout.margins: 0
                    running: false
                }
            }
        }


        model: productModel
        delegate: ProductDelegate {}
    }

    ListModel {
        id: productModel
    }

    WorkerScript {
        id: searchWorker
        source: "qrc:/js/product_request.mjs"
        property var busyIndicator
        onMessage: {
            busyIndicator.running = false
            print("Holalala");
            print('running '+busyIndicator.running);
        }
    }

    function addSuggetions(text, busyIndicator) {
        searchWorker.busyIndicator = busyIndicator
        busyIndicator.running = true;
        searchWorker.sendMessage({"name": text, "model": productModel});
    }
}
