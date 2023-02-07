class Request {
    constructor(url) {
        this.request_url = url;
    }

    request(verb, endpoint, obj, cb) {
        const complete_url = this.request_url + endpoint;
        print(`complete_url: ${complete_url}`);

        let xhr = new XMLHttpRequest();
        xhr.open(verb, complete_url);
        xhr.onload = function () {
            if (cb) {
                const str = xhr.responseText.toString();
                cb(JSON.parse(str));
            }
        }

        xhr.onerror = () => {
            if (cb)
                cb({ "code": "no_internet" });
        }

        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.setRequestHeader('Accept', 'application/json');

        let data = obj ? JSON.stringify(obj) : '';
        xhr.send(data);
    }
}

export { Request };
