import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import QtCharts
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
        const customersHandler = (data) => {
            const months = [];
            const orderQty = [];
            const locale = Qt.locale();
            let minDate = new Date().getTime();
            let maxDate = 0;
            for (let element of data.month_vs_order_qty) {
                const currDate = new Date(element.year, element.month)
                const currTime = currDate.getTime();
                if (currTime < minDate)
                    minDate = currTime;
                if (currTime > maxDate)
                    maxDate = currTime;

                barSeries.upperSeries.append(currDate, element.order_qty)

                orderQty.push(element.order_qty);
            }
            dateTimeAxis.min = new Date(2021, 01, 01);
            dateTimeAxis.max = new Date(2022, 01, 01);
            dateTimeAxis.tickCount = 5;

            barSeries.axisY.max = orderQty.reduce((a, b) => Math.max(a, b));

            //valueAxis.max = orderQty.reduce((a, b) => Math.max(a, b), -Infinity);

        }

        const currentYear = new Date().getFullYear();
        StatisticsResource.fetchData("/orders/?start_year=2020&end_year="+currentYear, customersHandler);

        const paymentsHandler = data => {
            const stats = {Credito: 0, Abonado: 0, Pago: 0};
            for (let el of data.statistics) {
                stats[el.status] += el.qty;
            }
            for (let key of Object.keys(stats))
                pieSeries.append(key, stats[key]);
        }

        StatisticsResource.fetchData("/payment_status/", paymentsHandler);
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        ColumnLayout {
            width: root.width
            ChartView {
                id: chartView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: root.height * .85
                title: "Meses vs Cantidad de ordenes"
                legend.alignment: Qt.AlignBottom
                antialiasing: true
                animationOptions: ChartView.SeriesAnimations
                animationEasingCurve.type: Easing.InOutQuad

                DateTimeAxis {
                    id: dateTimeAxis
                    format: "MMM yyyy"
                }

                AreaSeries {
                    id: barSeries
                    axisX: dateTimeAxis
                    axisY: ValueAxis {}
                    upperSeries: LineSeries {}
                }

                PinchArea{
                    id: pa
                    anchors.fill: parent
                    property real currentPinchScaleX: 1
                    property real currentPinchScaleY: 1
                    property real pinchStartX : 0
                    property real pinchStartY : 0

                    onPinchStarted: {
                        // Pinching has started. Record the initial center of the pinch
                        // so relative motions can be reversed in the pinchUpdated signal
                        // handler
                        pinchStartX = pinch.center.x;
                        pinchStartY = pinch.center.y;
                    }

                    onPinchUpdated: {
                        chartView.zoomReset();

                        // Reverse pinch center motion direction
                        var center_x = pinchStartX + (pinchStartX - pinch.center.x);
                        var center_y = pinchStartY + (pinchStartY - pinch.center.y);

                        // Compound pinch.scale with prior pinch scale level and apply
                        // scale in the absolute direction of the pinch gesture
                        var scaleX = currentPinchScaleX * (1 + (pinch.scale - 1) * Math.abs(Math.cos(pinch.angle * Math.PI / 180)));
                        var scaleY = currentPinchScaleY * (1 + (pinch.scale - 1) * Math.abs(Math.sin(pinch.angle * Math.PI / 180)));

                        // Apply scale to zoom levels according to pinch angle
                        var width_zoom = height / scaleX;
                        var height_zoom = width / scaleY;

                        var r = Qt.rect(center_x - width_zoom / 2, center_y - height_zoom / 2, width_zoom, height_zoom);
                        chartView.zoomIn(r);
                    }

                    onPinchFinished: {
                        // Pinch finished. Record compounded pinch scale.
                        currentPinchScaleX = currentPinchScaleX * (1 + (pinch.scale - 1) * Math.abs(Math.cos(pinch.angle * Math.PI / 180)));
                        currentPinchScaleY = currentPinchScaleY * (1 + (pinch.scale - 1) * Math.abs(Math.sin(pinch.angle * Math.PI / 180)));
                    }

                    MouseArea{
                        anchors.fill: parent
                        drag.target: dragTarget
                        drag.axis: Drag.XAndYAxis

                        onDoubleClicked: {
                            chartView.zoomReset();
                            parent.currentPinchScaleX = 1;
                            parent.currentPinchScaleY = 1;
                        }
                    }

                    Item {
                        // A virtual item to receive drag signals from the MouseArea.
                        // When x or y properties are changed by the MouseArea's
                        // drag signals, the ChartView is scrolled accordingly.
                        id: dragTarget

                        property real oldX : x
                        property real oldY : y

                        onXChanged: {
                            chartView.scrollLeft( x - oldX );
                            oldX = x;
                        }
                        onYChanged: {
                            chartView.scrollUp( y - oldY );
                            oldY = y;
                        }
                     }
                 }
            }

            ToolSeparator {
                orientation: Qt.Horizontal
                Layout.fillWidth: true
            }

            ChartView {
                title: "Estado de pago de las ordenes"
                legend.alignment: Qt.AlignBottom
                antialiasing: true
                Layout.preferredHeight: root.height * .8
                Layout.fillWidth: true
                Layout.fillHeight: true

                PieSeries {
                    id: pieSeries
                }
            }
        } // ColumnLayout
    } // ScrollView
}
