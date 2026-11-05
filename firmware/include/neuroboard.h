// firmware/include/neuroboard.h : broches de la NeuroBoard-S3 utilisees par les TP.
//
// Source : docs-NeuroboardS3 (pages "Brochage", "Contraintes des broches", "Camera").
// Camera branchee, GPIO4 a GPIO13, GPIO15 a GPIO18 sont pris. Broches libres : 1, 2, 14, 21, 42, 47, 48.
// Seules GPIO1 (ADC1_CH0) et GPIO2 (ADC1_CH1) sont des entrees ADC1 libres de tout peripherique.

#pragma once

#define NB_PIN_RGB_LED   38   // LED adressable (neopixelWrite)
#define PIN_SCOPE     21   // sortie inversee a chaque echantillon, pour l'oscilloscope
#define PIN_ADC       1    // entree analogique (ADC1_CH0) : potentiometre ou GBF
#define PIN_BOOT      0    // bouton BOOT (strapping, actif bas)

// Camera (DVP 8 bits + SCCB). Attention : D0..D7 ne sont pas dans l'ordre des GPIO.
#define CAM_PIN_PWDN  -1
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK  16
#define CAM_PIN_SIOD  4
#define CAM_PIN_SIOC  5
#define CAM_PIN_D7    15
#define CAM_PIN_D6    17
#define CAM_PIN_D5    18
#define CAM_PIN_D4    9
#define CAM_PIN_D3    11
#define CAM_PIN_D2    13
#define CAM_PIN_D1    12
#define CAM_PIN_D0    10
#define CAM_PIN_VSYNC 6
#define CAM_PIN_HREF  7
#define CAM_PIN_PCLK  8
