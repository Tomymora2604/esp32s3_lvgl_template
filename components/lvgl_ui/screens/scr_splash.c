#include "scr_splash.h"
#include "screens.h"
#include "ui.h"
#include "bsp_display.h"
#include "navbar.h" 

/* Animación de puntos: el valor va de 0 a 4 y elegimos el texto */
static void dots_anim_cb(void *var, int32_t v)
{
    static const char *dots[] = {"", ".", "..", "..."};
    lv_label_set_text(ui_lblDots, dots[v % 4]);
}

/* Fade-in: la animación sube el brillo de 0 a 100 */
static void backlight_anim_cb(void *var, int32_t v)
{
    bsp_display_set_brightness((uint8_t)v);
}

/* Tocar cualquier parte de la pantalla -> ir a Home */
static void on_screen_clicked(lv_event_t *e)
{
    ui_screens_navigate(SCREEN_HOME);
}

void scr_splash_prepare(void)
{
    lv_obj_add_flag(ui_scrSplash, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui_scrSplash, on_screen_clicked, LV_EVENT_CLICKED, NULL);
}

void scr_splash_init(void)
{
        navbar_set_active(-1);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_lblDots);
    lv_anim_set_exec_cb(&a, dots_anim_cb);
    lv_anim_set_values(&a, 0, 4);
    lv_anim_set_time(&a, 1200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

    bsp_display_set_brightness(0);
    lv_anim_t b;
    lv_anim_init(&b);
    lv_anim_set_var(&b, ui_scrSplash);
    lv_anim_set_exec_cb(&b, backlight_anim_cb);
    lv_anim_set_values(&b, 0, 100);
    lv_anim_set_time(&b, 800);
    lv_anim_start(&b);
}

void scr_splash_uninit(void)
{   
    lv_anim_del(ui_lblDots, dots_anim_cb);
    lv_anim_del(ui_scrSplash, backlight_anim_cb);
    bsp_display_set_brightness(100);
}

void scr_splash_step(void) { }