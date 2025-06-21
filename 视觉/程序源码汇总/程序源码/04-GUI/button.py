import lvgl as lv
import lvgl_helper as lv_h
import lcd
import time
from machine import Timer
import touchscreen as ts


lcd.init()
ts.init()
lv.init()

disp_buf1 = lv.disp_buf_t()
buf1_1 = bytearray(320*10)
lv.disp_buf_init(disp_buf1, buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
lv.disp_drv_init(disp_drv)
disp_drv.buffer = disp_buf1
disp_drv.flush_cb = lv_h.flush
disp_drv.hor_res = 320
disp_drv.ver_res = 240
lv.disp_drv_register(disp_drv)


indev_drv = lv.indev_drv_t()
lv.indev_drv_init(indev_drv)
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = lv_h.read
lv.indev_drv_register(indev_drv)


btn_count = 0
def on_btn_cb(obj, event):
    global btn_count
    if event == lv.EVENT.CLICKED:
        btn_count += 1
        label.set_text(str(btn_count))
        print("Button Press:", btn_count)

scr = lv.obj()
btn = lv.btn(scr)
btn.align(lv.scr_act(), lv.ALIGN.CENTER, 0, 0)
label = lv.label(btn)
label.set_text("Button")
label.set_size(20,20)
btn.set_event_cb(on_btn_cb)

lv.scr_load(scr)

tim = time.ticks_ms()
while True:
    if time.ticks_ms()-tim > 5:
        tim = time.ticks_ms()
        lv.task_handler()
        lv.tick_inc(5)
