import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Layouts
import Utils
import Product

Page {
    id: root

    required property int salespersonId
    readonly property T.StackView stackView: StackView.view

    title: "Estadisticas"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }
    Component.onCompleted: {
        const now = new Date();
        const lastDate = new Date(now.getFullYear(), now.getMonth()-1, now.getDay(), now.getDate());
        const dateString = lastDate
            .toISOString()
            .slice(0, 19)
            .replace('T', ' ');
        const handler = resp => populate(resp.summary, summaryModel);
        StatisticsResource.fetchData(
            `/summary/${salespersonId}/?last_date=${dateString}`,
            handler
        );
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight

        ColumnLayout {
            id: columnLayout
            anchors {
                fill: parent
                margins: Math.max(scrollView.effectiveScrollBarWidth, 10)
            }
            spacing: 10

            Label {
                text: "Estadisticas del ultimo mes"
                horizontalAlignment: Label.AlignHCenter
                font.pointSize: 14
                font.bold: true
                Layout.fillWidth: true
            }

            Repeater {
                delegate: summaryDelegate
                model: summaryModel
            }
        }
    }

    Component {
        id: iconDelegate
        ItemDelegate {
            id: itemDelegate
            required property url imageSource
            required property string title
            required property url path

            implicitWidth: 200
            implicitHeight: 250
            background: Rectangle {
                color: "chartreuse"
            }

            Layout.fillWidth: true
            Layout.fillHeight: true

            onClicked: stackView.push(path, {salespersonId})

            Column {
                id: column
                anchors.centerIn: parent
                spacing: 10
                padding: 24

                Image {
                    id: image
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: imageSource
                    sourceSize: "80x80"
                }

                Label {
                    width: itemDelegate.width * .8
                    text: title
                    horizontalAlignment: Label.AlignHCenter
                    font.pointSize: 12
                    wrapMode: Label.WordWrap
                }
            }
        }
    }

    Component {
        id: sectionDelegate
        Item {
            required property string section
            width: listView.width * .9
            implicitHeight: label.implicitHeight * 1.1

            Label {
                id: label
                anchors.left: parent.left
                anchors.leftMargin: listView.width *.05
                text: section
                color: "red"

                font.pointSize: 14
                font.bold: true
            }
        }
    }

    Component {
        id: summaryDelegate
        Item {
            required property string title
            required property var value

            Layout.fillWidth: true
            implicitHeight: frame.implicitHeight

            Frame {
                id: frame
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter

                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Label {
                        text: title
                        font.pointSize: 11
                        wrapMode: Label.Wrap
                        Layout.preferredWidth: parent.width/2
                    }

                    Label {
                        text: value
                        wrapMode: Label.Wrap
                        rightPadding: frame.padding
                        horizontalAlignment: Label.AlignRight
                        font.pointSize: 12
                        font.bold: true
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: parent.width/2
                    }
                }
            }
        }
    }

    ListModel {
        id: model

        // ListElement {
        //     imageSource: "qrc:/icons/product_statistics.svg"
        //     title: "Estadisticas de Productos"
        //     path: "qrc:qt/qml/Statistics/ProductStatistics.qml"
        // }

        // ListElement {
        //     imageSource: "qrc:/icons/customer_statistics.svg"
        //     title: "Estadisticas de Clientes"
        //     path: "qrc:qt/qml/Statistics/CustomerStatistics.qml"
        // }

        // ListElement {
        //     imageSource: "statistics-color.svg"
        //     title: "Ordenes"
        //     path: "OrderStatistics.qml"
        // }
    }

    ListModel {
        id: summaryModel
    }

    function populate(data, model): void {
        model.append([
            {title: "Total de ventas", value: data.latest_sales.order_qty.toString(), sectionString: "Ventas"},
            {title: "Producto mas vendido", value: data.most_selling.name, sectionString: "Productos"},
        ]);
        const runningOuttaStocks = data.running_outta_stocks;
        model.append({title: "Producto punto a agotarse", value: runningOuttaStocks[0].product_name, sectionString: "Productos"});

        const paymentStatus = data.payment_status;
        const handler = text => (
            text === "Pago" ? "Cuentas pagadas" : text === "Abonado"
                            ? "Cuentas abonadas" : "Cuentas a credito"
        );
        for (let el of paymentStatus) {
            const title = handler(el.status);
            const value = el.qty.toString();
            model.append({title, value, sectionString: "Cuentas"});
        }
    }
}
