// cam_capture : capture d'images de la camera et envoi au PC par le port serie.
//
// Commandes (un caractere sur le port serie) :
//   c  capture et envoie une image
//   j  format JPEG QVGA (320x240), pour regarder
//   g  format niveaux de gris 96x96, l'entree des modeles
//   r  format RGB565 96x96
// Trame envoyee : une ligne "FRAME <largeur> <hauteur> <format> <octets>\n", puis les octets bruts.
// Le script tools/serial_tool.py (commande "capture") recoit et enregistre les images.

#include <Arduino.h>
#include "esp_camera.h"
#include "neuroboard.h"

static bool init_camera(pixformat_t fmt, framesize_t size) {
    esp_camera_deinit();
    camera_config_t cfg = {};
    cfg.pin_pwdn = CAM_PIN_PWDN;
    cfg.pin_reset = CAM_PIN_RESET;
    cfg.pin_xclk = CAM_PIN_XCLK;
    cfg.pin_sccb_sda = CAM_PIN_SIOD;
    cfg.pin_sccb_scl = CAM_PIN_SIOC;
    cfg.pin_d7 = CAM_PIN_D7;
    cfg.pin_d6 = CAM_PIN_D6;
    cfg.pin_d5 = CAM_PIN_D5;
    cfg.pin_d4 = CAM_PIN_D4;
    cfg.pin_d3 = CAM_PIN_D3;
    cfg.pin_d2 = CAM_PIN_D2;
    cfg.pin_d1 = CAM_PIN_D1;
    cfg.pin_d0 = CAM_PIN_D0;
    cfg.pin_vsync = CAM_PIN_VSYNC;
    cfg.pin_href = CAM_PIN_HREF;
    cfg.pin_pclk = CAM_PIN_PCLK;
    cfg.xclk_freq_hz = 20000000;                         // descendre a 10 MHz si l'image est instable
    cfg.ledc_timer = LEDC_TIMER_0;
    cfg.ledc_channel = LEDC_CHANNEL_0;
    cfg.pixel_format = fmt;
    cfg.frame_size = size;
    cfg.jpeg_quality = 12;
    cfg.fb_count = 2;
    cfg.fb_location = CAMERA_FB_IN_PSRAM;
    cfg.grab_mode = CAMERA_GRAB_LATEST;
    esp_err_t err = esp_camera_init(&cfg);
    if (err != ESP_OK) {
        Serial.printf("# init camera echouee : 0x%x\n", err);
        return false;
    }
    // Chauffe : l'exposition et la balance des blancs automatiques mettent plusieurs images a converger
    // (mesure : en RGB565, la luminosite moyenne passe de 48 a 70 sur 255 en 8 images).
    for (int i = 0; i < 15; i++) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) {
            esp_camera_fb_return(fb);
        }
    }
    sensor_t *s = esp_camera_sensor_get();
    Serial.printf("# camera prete, capteur PID 0x%x, format %d, taille %d\n", s ? s->id.PID : 0, fmt, size);
    return true;
}

static const char *fmt_name(pixformat_t f) {
    switch (f) {
        case PIXFORMAT_JPEG: return "jpeg";
        case PIXFORMAT_GRAYSCALE: return "gray";
        case PIXFORMAT_RGB565: return "rgb565";
        default: return "autre";
    }
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.printf("# PSRAM : %lu octets\n", (unsigned long)ESP.getPsramSize());
    init_camera(PIXFORMAT_JPEG, FRAMESIZE_QVGA);
}

void loop() {
    if (!Serial.available()) {
        return;
    }
    char cmd = Serial.read();
    if (cmd == 'j') {
        init_camera(PIXFORMAT_JPEG, FRAMESIZE_QVGA);
    } else if (cmd == 'g') {
        init_camera(PIXFORMAT_GRAYSCALE, FRAMESIZE_96X96);
    } else if (cmd == 'r') {
        init_camera(PIXFORMAT_RGB565, FRAMESIZE_96X96);
    } else if (cmd == 'c') {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("# capture echouee");
            return;
        }
        Serial.printf("FRAME %u %u %s %u\n", fb->width, fb->height, fmt_name(fb->format), fb->len);
        Serial.write(fb->buf, fb->len);
        Serial.flush();
        esp_camera_fb_return(fb);                        // obligatoire, sinon les tampons s'epuisent
    }
}
