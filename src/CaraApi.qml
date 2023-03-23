import QtQuick
import com.cara 1.0

Rest {
    property string endpoint: ""
    base: "http://127.0.0.1:5000/api" + endpoint
}
