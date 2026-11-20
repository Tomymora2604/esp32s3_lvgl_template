#include <stdio.h>
#include "scr_home.h"
#include "screens.h"
#include "ui.h"
#include "bsp_battery.h"
#include "driver/temperature_sensor.h"
#include "esp_timer.h"

static temperature_sensor_handle_t s_temp = NULL;
static lv_timer_t *s_timer = NULL;

/* Voltaje de la batería a porcentaje: 3.3 V = 0 %, 4.2 V = 100 % */
static int battery_percent(void)
{
    float v = 0;
    uint16_t raw = 0;
    bsp_battery_get_voltage(&v, &raw);
    int pct = (int)((v - 3.3f) / (4.2f - 3.3f) * 100.0f);
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    return pct;
}

static void refresh_values(void)
{
    char buf[32];

    int pct = battery_percent();
    lv_arc_set_value(ui_arcBattery, pct);
    lv_label_set_text_fmt(ui_lblBattery, "%d%%", pct);

    float t = 0;
    if (temperature_sensor_get_celsius(s_temp, &t) == ESP_OK) {
        snprintf(buf, sizeof(buf), "Temp: %.1f °C", t);
        lv_label_set_text(ui_lblTemp, buf);
    }

    int64_t s = esp_timer_get_time() / 1000000;  /* microsegundos -> segundos */
    lv_label_set_text_fmt(ui_lblUptime, "%02d:%02d:%02d",
                          (int)(s / 3600), (int)(s / 60 % 60), (int)(s % 60));
}

static void timer_cb(lv_timer_t *t) { refresh_values(); }

/* Evento de pantalla cargada: actualiza apenas entrás */
static void on_screen_loaded(lv_event_t *e) { refresh_values(); }

void scr_home_prepare(void)
{
    bsp_battery_init();

    temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 80);
    ESP_ERROR_CHECK(temperature_sensor_install(&cfg, &s_temp));
    ESP_ERROR_CHECK(temperature_sensor_enable(s_temp));

    lv_arc_set_range(ui_arcBattery, 0, 100);
    lv_arc_set_bg_angles(ui_arcBattery, 150, 30);            /* ángulo personalizado */
    lv_obj_clear_flag(ui_arcBattery, LV_OBJ_FLAG_CLICKABLE);  /* que no se mueva con el dedo */

    lv_obj_add_event_cb(ui_scrHome, on_screen_loaded, LV_EVENT_SCREEN_LOADED, NULL);
}

void scr_home_init(void)
{
    s_timer = lv_timer_create(timer_cb, 1000, NULL);  /* cada 1 segundo */
}

void scr_home_uninit(void)
{
    if (s_timer) {
        lv_timer_del(s_timer);
        s_timer = NULL;
    }
}

void scr_home_step(void) { }