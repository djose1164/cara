import QtQuick
import QtCore
import Cara.Utils
import Cara.Billing

Item {
    id: root

    property alias document: printer.document

    signal billPrinted(path: string)

    PdfOrderPrinter {
        id: printer
        onOrderPrinted: path => {
            fileUtils.removeOldestFiles(BillingSettings.maximumSavedPdfFiles,
                                        internal.docsPath,
                                        internal.nameFilter);
            root.billPrinted(path);
        }
    }

    FileUtils {
        id: fileUtils
    }

    QtObject {
        id: internal
        readonly property url docsPath: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        readonly property string nameFilter: "factura-*.pdf"
    }

    function printBill() {
        printer.print();
    }
}
