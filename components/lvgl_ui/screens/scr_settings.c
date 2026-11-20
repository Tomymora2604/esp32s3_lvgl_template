#include <stdbool.h>
#include "scr_settings.h"
#include "screens.h"
#include "screens_timeout.h"
#include "navbar.h"
#include "ui.h"
#include "bsp_display.h"

#define DEFAULT_BRIGHTNESS 100
#define SCREEN_OFF_MS      30000   /* 30 segundos */

static void apply_brightness(int v)
{
    bsp_display_set_brightness((uint8_t)v);
    lv_label_set_text_fmt(ui_lblBrightness, "Brillo: %d%%", v);
}

static void on_slider_changed(lv_event_t *e)
{
    apply_brightness(lv_slider_get_value(ui_sldBrightness));
}

static void on_switch_changed(lv_event_t *e)
{
    bool on = lv_obj_has_state(ui_swTimeout, LV_STATE_CHECKED);
    disp_timeout_set_off_ms(on ? SCREEN_OFF_MS : TIMEOUT_NEVER);
}

/* Lo llama el boton fisico estando en esta pantalla */
void scr_settings_restore_defaults(void)
{
    lv_slider_set_value(ui_sldBrightness, DEFAULT_BRIGHTNESS, LV_ANIM_ON);
    apply_brightness(DEFAULT_BRIGHTNESS);
    lv_obj_clear_state(ui_swTimeout, LV_STATE_CHECKED);
    disp_timeout_set_off_ms(TIMEOUT_NEVER);
}

void scr_settings_prepare(void)
{
    lv_slider_set_range(ui_sldBrightness, 10, 100);
    /* Que mover el slider no cuente como "deslizar para cambiar de pantalla" */
    lv_obj_clear_flag(ui_sldBrightness, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(ui_sldBrightness, on_slider_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_swTimeout, on_switch_changed, LV_EVENT_VALUE_CHANGED, NULL);

    lv_label_set_text(ui_lblBuild, "Compilado: " __DATE__ " " __TIME__);

    lv_obj_clear_flag(ui_scrSettings, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_scrSettings, navbar_gesture_cb, LV_EVENT_GESTURE, NULL);
}

void scr_settings_init(void)
{
    navbar_set_active(2);
    int b = bsp_display_get_brightness();
    lv_slider_set_value(ui_sldBrightness, b, LV_ANIM_OFF);
    lv_label_set_text_fmt(ui_lblBrightness, "Brillo: %d%%", b);
}

void scr_settings_uninit(void) { }
void scr_settings_step(void) { }