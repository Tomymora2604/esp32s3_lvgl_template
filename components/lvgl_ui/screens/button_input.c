#include "button_input.h"
#include "iot_button.h"
#include "button_gpio.h"
#include "screens_update.h"
#include "esp_log.h"

#define BOOT_BUTTON_GPIO 0   /* boton BOOT de la placa */

static const char *TAG = "button";

/* Esto corre en la tarea del boton: NO se puede tocar LVGL aca.
   Solo mandamos un mensaje a la cola de la UI. */
static void on_click(void *btn, void *usr_data)
{
    ui_update_cmd(UI_UPDATE_BUTTON, NULL);
}

void button_input_init(void)
{
    const button_config_t btn_cfg = {0};
    const button_gpio_config_t gpio_cfg = {
        .gpio_num = BOOT_BUTTON_GPIO,
        .active_level = 0,
    };
    button_handle_t btn = NULL;
    if (iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &btn) == ESP_OK) {
        iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, on_click, NULL);
    } else {
        ESP_LOGE(TAG, "No se pudo crear el boton");
    }
}