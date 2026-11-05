// tp1_register_read : lecture du compteur d'un timer au niveau registre (TP 1, E6).
// Sequence du TRM, section 12.2.2 : ecrire TIMG_TxUPDATE_REG, attendre que le materiel le remette a 0,
// puis lire TIMG_TxLO_REG (32 bits bas) et TIMG_TxHI_REG (22 bits hauts).
// Compare avec gptimer_get_raw_count() : les deux valeurs doivent croitre ensemble.

#include <Arduino.h>
#include "driver/gptimer.h"
#include "soc/timer_group_struct.h"

static gptimer_handle_t timer = nullptr;

static uint64_t read_timer_counter() {
    // TODO : sequence de lecture sur TIMERG0.hw_timer[0] (noms des champs : voir soc/timer_group_struct.h)
    return 0;
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    gptimer_config_t cfg = {};
    cfg.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    cfg.direction = GPTIMER_COUNT_UP;
    cfg.resolution_hz = 1000000;
    ESP_ERROR_CHECK(gptimer_new_timer(&cfg, &timer));
    ESP_ERROR_CHECK(gptimer_enable(timer));
    ESP_ERROR_CHECK(gptimer_start(timer));
}

void loop() {
    Serial.println("\n  registre (TIMERG0 T0)   gptimer_get_raw_count   ecart");
    for (int i = 0; i < 10; i++) {
        uint64_t reg = read_timer_counter();
        uint64_t drv = 0;
        ESP_ERROR_CHECK(gptimer_get_raw_count(timer, &drv));
        Serial.printf("  %20llu   %20llu   %+lld\n", reg, drv, (long long)(drv - reg));
        delay(100);
    }
    delay(3000);
}
