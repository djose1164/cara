pragma Singleton
import QtQuick
import com.cara

QtObject {
    id: root

    readonly property BillModel model: BillModel {}
}
