#include <stdio.h>
#include "esp_timer.h"
#include "esp_pm.h"
#include "esp_wifi.h"
// #include "esp_bt.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "benchmark.h"

// #define CPU_FREQ_MHZ 80
// #define CPU_FREQ_MHZ 160
#define CPU_FREQ_MHZ 240  // cambiar a 80 / 160 / 240 para pruebas

void benchmark_task(void *arg) {
    int64_t start, end;
    
    for (int i = 0; i < 5; i++) {
        // Test enteros
        start = esp_timer_get_time();
        test_integers();
        end = esp_timer_get_time();
        printf("INT,%lld\n", end - start);
    
        // Test floats
        start = esp_timer_get_time();
        test_floats();
        end = esp_timer_get_time();
        printf("FLOAT,%lld\n", end - start);
    }

    vTaskDelete(NULL);
}

void app_main() {
    // 1. Desactivar watchdog
    esp_task_wdt_deinit();

    // 2. Desactivar WiFi y Bluetooth
    esp_wifi_stop();
    // esp_bt_controller_disable();

    // 3. Fijar frecuencia CPU
    esp_pm_config_t pm_config = {
        .max_freq_mhz = CPU_FREQ_MHZ,
        .min_freq_mhz = CPU_FREQ_MHZ,
        .light_sleep_enable = false
    };
    esp_pm_configure(&pm_config);

    // 4. Crear tarea fijada a un core (CPU 0)
    xTaskCreatePinnedToCore(
        benchmark_task,
        "benchmark_task",
        8192,
        NULL,
        1,
        NULL,
        0   // core 0
    );
}