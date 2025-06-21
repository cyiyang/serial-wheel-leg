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
lv.disp_buf_init(disp_buf1,buf1_1, None, len(buf1_1)//4)
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


def on_slider_changed(self, obj=None, event=-1):
    slider_value = slider.get_value()
    textView.set_text("Value: %d" % (slider_value))
    print("slider:", slider_value)

# Create a slider
scr = lv.obj()
slider = lv.slider(scr)
slider.align(lv.scr_act(), lv.ALIGN.CENTER, 0, 0)
slider.set_width(200)
slider.set_height(30)
slider.set_range(0, 100)
slider.set_value(0, 0)
slider.set_event_cb(on_slider_changed)


# Create a label
textView = lv.label(scr)
textView.align(lv.scr_act(), lv.ALIGN.CENTER, -50, -50)
textView.set_text("Value:0")

lv.scr_load(scr)


tim = time.ticks_ms()
while True:
    if time.ticks_ms()-tim > 5:
        tim = time.ticks_ms()
        lv.task_handler()
        lv.tick_inc(5)

