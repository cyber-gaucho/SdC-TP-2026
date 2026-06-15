from flask import Flask, render_template, jsonify, request
import threading
import time

app = Flask(__name__)

DEVICE = "/dev/tp5"

selected_channel = "1"

BUFFER_SIZE = 300
SAMPLE_PERIOD = 0.02

samples = []
timestamps = []

frequency = 0.0
duty_cycle = 0.0

lock = threading.Lock()


def select_channel(ch):
    with open(DEVICE, "w") as dev:
        dev.write(ch)


def read_sample():
    with open(DEVICE, "r") as dev:
        value = dev.read(1)

    return 1 if value == "1" else 0


def sampler_thread():
    global frequency
    global duty_cycle

    last_value = None
    last_rising_edge = None

    while True:

        now = time.time()
        value = read_sample()

        with lock:

            samples.append(value)
            timestamps.append(now)

            if len(samples) > BUFFER_SIZE:
                samples.pop(0)
                timestamps.pop(0)

            highs = sum(samples)

            if len(samples) > 0:
                duty_cycle = 100.0 * highs / len(samples)

            if (
                last_value is not None
                and last_value == 0
                and value == 1
            ):
                if last_rising_edge is not None:

                    period = now - last_rising_edge

                    if period > 0:
                        frequency = 1.0 / period

                last_rising_edge = now

            last_value = value

        time.sleep(SAMPLE_PERIOD)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/channel", methods=["POST"])
def channel():
    global selected_channel
    global samples
    global timestamps
    global frequency
    global duty_cycle

    ch = request.json["channel"]

    if ch not in ["1", "2"]:
        return jsonify({"status": "error"})

    select_channel(ch)

    with lock:
        selected_channel = ch
        samples = []
        timestamps = []
        frequency = 0.0
        duty_cycle = 0.0

    return jsonify({"status": "ok"})


@app.route("/api/data")
def data():

    with lock:

        if len(timestamps) == 0:
            return jsonify({})

        t0 = timestamps[0]

        return jsonify({
            "channel": selected_channel,
            "frequency": round(frequency, 2),
            "duty": round(duty_cycle, 1),
            "samples": samples,
            "time": [
                round(t - t0, 2)
                for t in timestamps
            ]
        })


if __name__ == "__main__":

    select_channel("1")

    threading.Thread(
        target=sampler_thread,
        daemon=True
    ).start()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )
