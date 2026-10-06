#!/usr/bin/env python3

import time
import sys

DEVICE = "/dev/tp5"

WINDOW = 100
SAMPLE_PERIOD = 0.01

channel = "1"

samples = []
last = None
edges = 0

start_time = time.time()

if len(sys.argv) > 1:
    if sys.argv[1] in ("1", "2"):
        channel = sys.argv[1]

print(f"Canal seleccionado: GPIO{'17' if channel == '1' else '27'}")

try:

    while True:

        #
        # Seleccionar canal
        #
        with open(DEVICE, "w") as dev:
            dev.write(channel)

        #
        # Leer muestra
        #
        with open(DEVICE, "r") as dev:
            value = dev.read(1)

        if value not in ("0", "1"):
            continue

        value = int(value)

        samples.append(value)

        if len(samples) > WINDOW:
            samples.pop(0)

        if last is not None and value != last:
            edges += 1

        last = value

        elapsed = time.time() - start_time

        freq = 0.0

        if elapsed > 0:
            freq = (edges / 2.0) / elapsed

        duty = (
            100.0 * sum(samples) / len(samples)
            if samples else 0
        )

        graph = "".join(
            "█" if s else "_"
            for s in samples
        )

        sys.stdout.write("\033[2J\033[H")

        print("TP5 GPIO Monitor")
        print("================")
        print()

        print(
            f"Canal: GPIO{'17' if channel == '1' else '27'}"
        )

        print(
            f"Frecuencia estimada: {freq:.2f} Hz"
        )

        print(
            f"Duty cycle: {duty:.1f} %"
        )

        print(
            f"Muestras: {len(samples)}"
        )

        print()
        print(graph)

        time.sleep(SAMPLE_PERIOD)

except KeyboardInterrupt:
    print("\nFin")