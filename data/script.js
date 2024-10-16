setInterval(function () {
  refreshValue("time");
  refreshValue("log");
  refreshPinA("0");
}, 1000);

function refreshPinA(pin) {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pina" + pin).innerHTML = this.responseText;
    }
  };
  xhr.open("GET", "/pins-a?pin=" + pin, true);
  xhr.send();
}

function refreshValue(elementid) {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(elementid).innerHTML = this.responseText;
    }
  };
  xhr.open("GET", "/" + elementid, true);
  xhr.send();
}

function toggleCheckbox(element) {
  console.log("toggleCheckbox: " + element.id);
  var xhr = new XMLHttpRequest();
  xhr.open(
    "GET",
    "/set?element=" + element.id + "&value=" + (element.checked ? "1" : "0"),
    true
  );
  xhr.send();
}

function sendTime() {
  var year = document.getElementById("year").value;
  var month = document.getElementById("month").value;
  var day = document.getElementById("day").value;
  var hours = document.getElementById("hours").value;
  var minutes = document.getElementById("minutes").value;

  var xhr = new XMLHttpRequest();
  xhr.open(
    "GET",
    "/time-set?year=" +
      year +
      "&month=" +
      month +
      "&day=" +
      day +
      "&hours=" +
      hours +
      "&minutes=" +
      minutes,
    true
  );
  xhr.send();
}
