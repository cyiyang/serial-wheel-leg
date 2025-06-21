import sensor, image, time, lcd
from maix import KPU

from modules import ybserial
import time

serial = ybserial()

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)
clock = time.clock()

od_img = image.Image(size=(320,256))

anchor = (0.893, 1.463, 0.245, 0.389, 1.55, 2.58, 0.375, 0.594, 3.099, 5.038, 0.057, 0.090, 0.567, 0.904, 0.101, 0.160, 0.159, 0.255)
kpu = KPU()
kpu.load_kmodel("/sd/KPU/yolo_face_detect/yolo_face_detect.kmodel")
kpu.init_yolo2(anchor, anchor_num=9, img_w=320, img_h=240, net_w=320, net_h=256, layer_w=10, layer_h=8, threshold=0.7, nms_value=0.3, classes=1)


x=""
y=""
h=""
w=""

while True:
    clock.tick()
    img = sensor.snapshot()
    a = od_img.draw_image(img, 0,0)
    od_img.pix_to_ai()
    kpu.run_with_output(od_img)
    dect = kpu.regionlayer_yolo2()
    fps = clock.fps()
    if len(dect) > 0:
        print("dect:",dect)
        for l in dect :
            a = img.draw_rectangle(l[0],l[1],l[2],l[3], color=(0, 255, 0))
            x = str(l[0])
            y = str(l[1])
            w = str(l[2])
            h = str(l[3])
            if len(x)<3 :
               i_flag = 3-len(x)
               x = "0"*i_flag + x
            if len(y)<3 :
               i_flag = 3-len(y)
               y = "0"*i_flag + y
            if len(w)<3 :
               i_flag = 3-len(w)
               w = "0"*i_flag + w
            if len(h)<3 :
               i_flag = 3-len(h)
               h = "0"*i_flag + h
    a = img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)




    if len(dect) > 0:
        send_data ="$"+"05"+ x+','+ y+','+ w+','+ h+','+"#"
        time.sleep_ms(5)
        serial.send(send_data)
    lcd.display(img)

kpu.deinit()
