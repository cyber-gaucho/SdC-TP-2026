#include <stdio.h>
#include "esp_timer.h"
#include "benchmark.h"

void app_main() {
    int64_t start, end;

    start = esp_timer_get_time();
    test_integers();
    end = esp_timer_get_time();
    printf("INT,%lld\n", end - start);

    start = esp_timer_get_time();
    test_floats();
    end = esp_timer_get_time();
    printf("FLOAT,%lld\n", end - start);
}