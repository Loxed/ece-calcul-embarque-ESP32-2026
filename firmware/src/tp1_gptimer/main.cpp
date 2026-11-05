// tp1_gptimer : alarme periodique avec le driver ESP-IDF GPTimer (TP 1, E2).
// Parcourt 8, 16, 32, 44.1 et 48 kHz, 5 s chacune. Pour chaque frequence : alarm_count, frequence
// calculee, et frequence mesuree en comptant les alarmes. PIN_SCOPE s'inverse a chaque alarme.

#include <Arduino.h>
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "neuroboard.h"

static const uint32_t RESOLUTION_HZ = 1000000;          // 1 tick = 1 us
static const uint32_t FREQS[] = {8000, 16000, 32000, 44100, 48000};

static gptimer_handle_t timer = nullptr;
static volatile uint32_t alarms = 0;
static volatile uint32_t level = 0;

static bool IRAM_ATTR on_alarm(gptimer_handle_t, const gptimer_alarm_event_data_t *, void *) {
    // TODO : inverser PIN_SCOPE avec gpio_set_level() et incrementer alarms
    return false;                                        // pas de tache a reveiller
}

static void set_frequency(uint32_t fs) {
    uint64_t count = 0;                                  // TODO : alarm_count pour la frequence fs
    // TODO : remplir un gptimer_alarm_config_t (alarm_count, reload_count = 0,
    //        flags.auto_reload_on_alarm = true) et appeler gptimer_set_alarm_action()
    double actual = count ? (double)RESOLUTION_HZ / count : 0;
    Serial.printf("\nvisee %6lu Hz | alarm_count %3llu | calculee %9.2f Hz\n", (unsigned long)fs, count, actual);
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    gpio_reset_pin((gpio_num_t)PIN_SCOPE);
    gpio_set_direction((gpio_num_t)PIN_SCOPE, GPIO_MODE_OUTPUT);

    // TODO : creer le timer avec gptimer_new_timer() (clk_src GPTIMER_CLK_SRC_DEFAULT, comptage
    //        croissant, resolution_hz = RESOLUTION_HZ), enregistrer on_alarm avec
    //        gptimer_register_event_callbacks(), puis gptimer_enable()
    set_frequency(FREQS[0]);
    // TODO : gptimer_start()
}

void loop() {
    static size_t idx = 0;
    for (int s = 0; s < 5; s++) {
        uint32_t a0 = alarms;
        int64_t t0 = esp_timer_get_time();
        delay(1000);
        uint32_t a1 = alarms;
        int64_t t1 = esp_timer_get_time();
        Serial.printf("  mesuree : %.1f alarmes/s\n", (a1 - a0) * 1e6 / (double)(t1 - t0));
    }
    idx = (idx + 1) % (sizeof(FREQS) / sizeof(FREQS[0]));
    set_frequency(FREQS[idx]);
}
