window.onload = onLoad

function onLoad() {
    getColors();
    getDeepSleep();
    getTimeZone();
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
    const form = document.getElementById("colorsForm")
    const formData = new FormData(form);

    try {
        const response = await fetch("/colors", {
          method: "POST",
          body: formData,
        });
        console.log(await response.json());
      } catch (e) {
        console.error(e);
      }
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
    const form = document.getElementById("timeOffsetForm")
    const formData = new FormData(form);

    try {
        const response = await fetch("/timezone", {
          method: "POST",
          body: formData,
        });
        console.log(await response.json());
      } catch (e) {
        console.error(e);
      }
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
    const form = document.getElementById("deepSleepForm")
    const formData = new FormData(form);

    try {
        const response = await fetch("/deep-sleep", {
          method: "POST",
          body: formData,
        });
        console.log(await response.json());
      } catch (e) {
        console.error(e);
      }
}
