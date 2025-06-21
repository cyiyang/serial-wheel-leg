import lvgl as lv
import lvgl_helper as lv_h
import lcd, sensor, image
import time
from machine import Timer
import touchscreen as ts

from modules import ybrgb, ybkey, ybserial

RGB = ybrgb()
KEY = ybkey()
SER = ybserial()

lcd.init()
ts.init()
lv.init()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 500)
clock = time.clock()


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


rgb_state = False
btn_count = 0
def on_btn_cb(obj, event):
    global rgb_state
    if event == lv.EVENT.CLICKED:
        print("RGB Button Press:")
        rgb_state = True

ser_state = False
def on_btn1_cb(obj, event):
    global ser_state
    if event == lv.EVENT.CLICKED:
        print("SERIAL Button Press:")
        ser_state = True


camera_state = False
def on_btn2_cb(obj, event):
    global camera_state
    if event == lv.EVENT.CLICKED:
        print("CAMERA Button Press:")
        camera_state = True
        RGB.set(0, 0, 0)

scr = lv.obj()
# RGB灯
btn = lv.btn(scr)
btn.align(lv.scr_act(), lv.ALIGN.IN_TOP_RIGHT, -50, 30)
label = lv.label(btn)
label.set_text("RGB")
label.set_size(20,20)
btn.set_event_cb(on_btn_cb)

# 串口
btn1 = lv.btn(scr)
btn1.align(lv.scr_act(), lv.ALIGN.IN_TOP_LEFT, 50, 30)
label1 = lv.label(btn1)
label1.set_text("Serial")
label1.set_size(20,20)
btn1.set_event_cb(on_btn1_cb)

# 摄像头
btn2 = lv.btn(scr)
btn2.align(lv.scr_act(), lv.ALIGN.CENTER, 0, 40)
label2 = lv.label(btn2)
label2.set_text("Camera")
label2.set_size(20,20)
btn2.set_event_cb(on_btn2_cb)


lv.scr_load(scr)


def test_rgb(index):
    global rgb_state
    # 间隔时间，单位5ms，亮灯时间=inverval*5ms
    inverval = 60
    if index == 0:
        RGB.set(255, 0, 0)
    elif index == inverval:
        RGB.set(0, 255, 0)
    elif index == inverval*2:
        RGB.set(0, 0, 255)
    elif index == inverval*3:
        RGB.set(255, 255, 255)
    elif index == inverval*4:
        RGB.set(0, 0, 0)
        rgb_state = False

key_state = False
key_count = 0
def test_key(index):
    global key_state
    inverval = 10
    if index >= inverval:
        key_state = 0
        RGB.set(0, 0, 0)
    else:
        RGB.set(0, 255, 0)

def test_camera():
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 150), scale=2.0)
    lcd.display(img)
    print("fps=", fps)


tim = time.ticks_ms()
while True:
    if camera_state:
        test_camera()
        if KEY.is_press():
            camera_state = False
            lv.scr_load(scr)
    else:
        if time.ticks_ms()-tim > 5:
            tim = time.ticks_ms()
            lv.task_handler()
            lv.tick_inc(5)
            if rgb_state:
                test_rgb(btn_count)
                btn_count = btn_count + 1
            else:
                btn_count = 0
            if KEY.is_press():
                #print("key press:")
                key_state = True
                key_count = 0
            if key_state:
                test_key(key_count)
                key_count = key_count + 1

            if ser_state:
                SER.send("K210 serial test OK\n")
                ser_state = False



