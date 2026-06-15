const ctx =
    document.getElementById("signalChart");

let currentColor = "#facc15";

const chart = new Chart(ctx, {

    type: "line",

    data: {

        labels: [],

        datasets: [{
            label: "Signal",
            data: [],
            borderColor: currentColor,
            borderWidth: 3,
            tension: 0,
            stepped: true
        }]
    },

    options: {

        animation: false,

        responsive: true,

        scales: {

            x: {
                title: {
                    display: true,
                    text: "Tiempo (s)"
                }
            },

            y: {
                min: -0.1,
                max: 1.1,
                ticks: {
                    stepSize: 1
                }
            }
        }
    }
});


async function selectChannel(ch) {

    await fetch("/api/channel", {

        method: "POST",

        headers: {
            "Content-Type": "application/json"
        },

        body: JSON.stringify({
            channel: ch
        })
    });

    currentColor =
        (ch === "1")
        ? "#facc15"
        : "#f97316";

    chart.data.labels = [];
    chart.data.datasets[0].data = [];
    chart.data.datasets[0].borderColor = currentColor;

    chart.update();

    document.getElementById("channel").innerText =
        (ch === "1")
        ? "GPIO17"
        : "GPIO27";
}


async function updateData() {

    const response =
        await fetch("/api/data");

    const data =
        await response.json();

    if (!data.samples)
        return;

    chart.data.labels =
        data.time;

    chart.data.datasets[0].data =
        data.samples;

    chart.update();

    document.getElementById("freq").innerText =
        data.frequency.toFixed(2);

    document.getElementById("duty").innerText =
        data.duty.toFixed(1);

    const state =
        data.samples[data.samples.length - 1];

    document.getElementById("state").innerText =
        state ? "HIGH" : "LOW";
}


setInterval(
    updateData,
    500
);
