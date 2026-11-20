#include <stdio.h>
#include <stdint.h>
#include "scr_system.h"
#include "screens.h"
#include "navbar.h"
#include "ui.h"
#include "esp_system.h"
#include "esp_idf_version.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_private/esp_clk.h"

static uint32_t s_min_heap = UINT32_MAX;
static lv_timer_t *s_timer = NULL;

static void refresh(void)
{
    char buf[48];

    snprintf(buf, sizeof(buf), "CPU: %d MHz", esp_clk_cpu_freq() / 1000000);
    lv_label_set_text(ui_lblCpu, buf);

    uint32_t free_heap = esp_get_free_heap_size();
    if (free_heap < s_min_heap) s_min_heap = free_heap;
    snprintf(buf, sizeof(buf), "Heap libre: %lu KB", (unsigned long)(free_heap / 1024));
    lv_label_set_text(ui_lblHeap, buf);
    snprintf(buf, sizeof(buf), "Heap minimo: %lu KB", (unsigned long)(s_min_heap / 1024));
    lv_label_set_text(ui_lblHeapMin, buf);

    snprintf(buf, sizeof(buf), "IDF: %s", esp_get_idf_version());
    lv_label_set_text(ui_lblIdf, buf);

    esp_chip_info_t info;
    esp_chip_info(&info);
    snprintf(buf, sizeof(buf), "Chip: %s, %d nucleos, rev %d.%d",
             info.model == CHIP_ESP32S3 ? "ESP32-S3" : "otro",
             info.cores, info.revision / 100, info.revision % 100);
    lv_label_set_text(ui_lblChip, buf);

    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);
    snprintf(buf, sizeof(buf), "Flash: %lu MB", (unsigned long)(flash_size / (1024 * 1024)));
    lv_label_set_text(ui_lblFlash, buf);
}

static void timer_cb(lv_timer_t *t) { refresh(); }

/* Lo llama el boton fisico estando en esta pantalla */
void scr_system_reset_min_heap(void)
{
    s_min_heap = esp_get_free_heap_size();
    refresh();
}

void scr_system_prepare(void)
{
    lv_obj_clear_flag(ui_scrSystem, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_scrSystem, navbar_gesture_cb, LV_EVENT_GESTURE, NULL);
}

void scr_system_init(void)
{
    navbar_set_active(1);
    refresh();
    s_timer = lv_timer_create(timer_cb, 1000, NULL);
}

void scr_system_uninit(void)
{
    if (s_timer) {
        lv_timer_del(s_timer);
        s_timer = NULL;
    }
}

void scr_system_step(void) { }