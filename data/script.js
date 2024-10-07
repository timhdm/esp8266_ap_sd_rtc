setInterval(function () {
  refreshValue("time");
  refreshValue("pins");
}, 1000);

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
  xhr.open("GET", "/set?element=" + element.id + "&value=" + (element.checked ? "1" : "0"), true);
  xhr.send();
}


