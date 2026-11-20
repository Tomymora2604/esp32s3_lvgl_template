#pragma once
#include "lvgl.h"

void navbar_create(void);
void navbar_set_active(int idx);          /* 0=Inicio 1=Sistema 2=Ajustes, -1=ocultar */
void navbar_gesture_cb(lv_event_t *e);    /* deslizar el dedo para cambiar de pantalla */