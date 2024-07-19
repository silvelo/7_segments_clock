window.onload = onLoad

debugMode = false;

function onLoad() {
    getColors();
    getDeepSleep();
    getLedsPerSegment();
    getTimeZone();
    getDebug()
}

function getColors() {
    fetch("/colors")
        .then((response) => response.json())
        .then((data) => {
            document.getElementById("hour_led1_color").value =
                data.hour_led1_color;
            document.getElementById("hour_led2_color").value =
                data.hour_led2_color;
            document.getElementById("dots_led1_color").value =
                data.dots_led1_color;
            document.getElementById("dots_led2_color").value =
                data.dots_led2_color;
            document.getElementById("minutes_led1_color").value =
                data.minutes_led1_color;
            document.getElementById("minutes_led2_color").value =
                data.minutes_led2_color;
        });
}

async function setColors(event){
    event.preventDefault();
    const formElement = document.getElementById("colorsForm")

    await setData("/colors", formElement);
}

function getDebug() {
    fetch("/debug")
        .then((response) => response.json())
        .then((data) => {
            debugMode =            data.debug;
        });
}

async function setDebug(){
    debugMode = !debugMode;
    console.log(debugMode)
    await makePost("/debug", {debug: debugMode});
}

function getLedsPerSegment() {
    fetch("/leds")
        .then((response) => response.json())
        .then((data) => {

            document.getElementById("ledsPerSegment").value =
                data.ledsPerSegment;
        });
}

async function setLedsPerSegment(event){
    event.preventDefault();
    const formElement = document.getElementById("ledsPerSegmentForm")
   
    await setData("/leds", formElement);    
}

function getTimeZone() {
    fetch("/timezone")
        .then((response) => response.json())
        .then((data) => {

            document.getElementById("timeOffset").value =
                data.timeOffset;
        });
}

async function setTimezone(event){
    event.preventDefault();
    const formElement = document.getElementById("timeOffsetForm")
   
    await setData("/timezone", formElement);    
}

function getDeepSleep() {
    fetch("/deep-sleep")
        .then((response) => response.json())
        .then((data) => {
            document.getElementById("start_timestamp").value = data.start_timestamp;
            document.getElementById("end_timestamp").value = data.end_timestamp;
        });
}

async function setDeepSleep(event){
    event.preventDefault();
    const formElement = document.getElementById("deepSleepForm")
    
    await setData("/deep-sleep", formElement);

}

async function makePost(url, data) {
    try {
      const response = await fetch(url, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });
      //console.log(await response.json());
    } catch (e) {
      console.error(e);
    }
  }

async function setData(url, formElement) {
  const formData = new FormData(formElement);

  try {
    const response = await fetch(url, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(Object.fromEntries(formData)),
    });
    //console.log(await response.json());
  } catch (e) {
    console.error(e);
  }
}