import QtQuick
import com.cara 1.0

Rest {
    property string endpoint: ""
    base: Constants.base + endpoint
}
