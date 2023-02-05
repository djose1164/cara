
/*function testRequest() {
    let xhr = new XMLHttpRequest();

    xhr.onreadystatechange = function() {
        print('xhr: on ready state change: ' + xhr.readyState)
        if(xhr.readyState === XMLHttpRequest.DONE) {
            print('xhr: on ready DONE change: ' + xhr.readyState)
            print(xhr.responseText);
            //WorkerScript.sendMessage({"obj": "s"})
        }
    }

    xhr.open("POST", "https://reqbin.com/echo/post/json");

    let data = {
      "Id": 78912,
      "Customer": "Jason Sweet",
    };

    xhr.onload = () => console.log(xhr.status);


    xhr.send(data);
   // WorkerScript.sendMessage({"response": "hola"});
}*/

export function hola() {
    let xhr = new XMLHttpRequest;
    xhr.open("GET", "https://api.coindesk.com/v1/bpi/currentprice.json");
    if ("")
    xhr.onreadystatechange = function () {
        print("xhr: state: " + xhr.readyState);
        if (xhr.readyState === XMLHttpRequest.DONE
                && xhr.status == 200) {
             print(xhr.responseText.toString());
        }
    }
    xhr.send(null);
}

/*WorkerScript.onMessage = () => {
    hola();
    print("exitin worker");
}*/
