pragma Singleton
import QtQuick
import Cara.Data

QtObject {
    function fetchData(endpoint, callback) {
        const base = Constants.base + "/statistics"+endpoint;
        const xhr = new XMLHttpRequest;
        xhr.onreadystatechange = () => {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                const response = xhr.responseText.toString();
                callback(JSON.parse(response))
            }
        }

        xhr.open("GET", base);
        xhr.send();
    }
}
