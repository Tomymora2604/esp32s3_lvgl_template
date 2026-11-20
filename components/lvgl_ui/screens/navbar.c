#include <stdint.h>
#include "navbar.h"
#include "screens.h"

static lv_obj_t *s_bar = NULL;
static lv_obj_t *s_btns[3];
static const ui_screens_t s_targets[3] = { SCREEN_HOME, SCREEN_SYSTEM, SCREEN_SETTINGS };
static const char *s_names[3] = { LV_SYMBOL_HOME " Inicio", LV_SYMBOL_LIST " Sistema", LV_SYMBOL_SETTINGS " Ajustes" };

static void on_nav_clicked(lv_event_t *e)
{
    ui_screens_t target = (ui_screens_t)(intptr_t)lv_event_get_user_data(e);
    ui_screens_navigate(target);
}

/* Se crea UNA sola vez, sobre la capa superior: queda encima de todas las pantallas */
void navbar_create(void)
{
    if (s_bar) return;
    s_bar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(s_bar, LV_PCT(100), 40);
    lv_obj_align(s_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_all(s_bar, 2, 0);
    lv_obj_set_style_radius(s_bar, 0, 0);
    lv_obj_clear_flag(s_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(s_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for (int i = 0; i < 3; i++) {
        s_btns[i] = lv_btn_create(s_bar);
        lv_obj_set_size(s_btns[i], 100, 34);
        lv_obj_add_event_cb(s_btns[i], on_nav_clicked, LV_EVENT_CLICKED, (void *)(intptr_t)s_targets[i]);
        lv_obj_t *lbl = lv_label_create(s_btns[i]);
        lv_label_set_text(lbl, s_names[i]);
        lv_obj_center(lbl);
    }
    lv_obj_add_flag(s_bar, LV_OBJ_FLAG_HIDDEN);
}

/* Pinta de azul el boton de la pantalla activa y de gris los demas */
void navbar_set_active(int idx)
{
    if (!s_bar) return;
    if (idx < 0) {
        lv_obj_add_flag(s_bar, LV_OBJ_FLAG_HIDDEN);
        return;
    }
    lv_obj_clear_flag(s_bar, LV_OBJ_FLAG_HIDDEN);
    for (int i = 0; i < 3; i++) {
        lv_color_t c = (i == idx) ? lv_palette_main(LV_PALETTE_BLUE) : lv_palette_main(LV_PALETTE_GREY);
        lv_obj_set_style_bg_color(s_btns[i], c, 0);
    }
}

/* Deslizar a la izquierda = siguiente pantalla, a la derecha = anterior */
void navbar_gesture_cb(lv_event_t *e)
{
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    ui_screens_t now = ui_screens_get_current();
    int cur = -1;
    for (int i = 0; i < 3; i++) {
        if (s_targets[i] == now) cur = i;
    }
    if (cur < 0) return;

    int next = cur;
    if (dir == LV_DIR_LEFT && cur < 2) next = cur + 1;
    else if (dir == LV_DIR_RIGHT && cur > 0) next = cur - 1;

    if (next != cur) {
        lv_indev_wait_release(lv_indev_get_act());
        ui_screens_navigate(s_targets[next]);
    }
}