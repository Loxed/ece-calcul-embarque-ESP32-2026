// esp32_adc_continuous.ino : acquisition ADC en mode continu (driver ESP-IDF), exemple du TP 0.
// Sur la NeuroBoard-S3, l'exemple du livret lit ADC_CHANNEL_4 (GPIO5), qui est pris par la camera
// (CAM_SIOC). Ce fichier lit ADC_CHANNEL_0 (GPIO1), libre camera branchee.

#include <Arduino.h>
#include "esp_adc/adc_continuous.h"

#define ADC_UNIT        ADC_UNIT_1
#define ADC_CHANNEL     ADC_CHANNEL_0     // GPIO1
#define SAMPLE_FREQ_HZ  20000             // 20 kHz d'échantillonnage
#define CONV_FRAME_SIZE 256               // Taille d'une trame, en octets

adc_continuous_handle_t adc_handle = NULL;

void setup() {
    Serial.begin(115200);

    // 1. Allocation des ressources du driver
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = CONV_FRAME_SIZE,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    // 2. Configuration du canal à échantillonner
    adc_digi_pattern_config_t adc_pattern = {
        .atten = ADC_ATTEN_DB_12,
        .channel = ADC_CHANNEL,
        .unit = ADC_UNIT,
        .bit_width = ADC_BITWIDTH_12,
    };

    adc_continuous_config_t dig_cfg = {
        .pattern_num = 1,
        .adc_pattern = &adc_pattern,
        .sample_freq_hz = SAMPLE_FREQ_HZ,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &dig_cfg));

    // 3. Démarrage de l'acquisition
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));
}

void loop() {
    adc_continuous_data_t samples[64];
    uint32_t num_samples = 0;

    // Lecture des échantillons déjà accumulés par le driver.
    // La fonction est bloquante au maximum 100 ms si aucun
    // échantillon n'est disponible.
    esp_err_t ret = adc_continuous_read_parse(
        adc_handle,
        samples,
        64,
        &num_samples,
        100
    );

    if (ret == ESP_OK) {
        for (int i = 0; i < num_samples; i++) {
            if (samples[i].valid) {
                Serial.printf(
                    "Canal %d : %lu\n",
                    samples[i].channel,
                    samples[i].raw_data
                );
            }
        }
    }
}
