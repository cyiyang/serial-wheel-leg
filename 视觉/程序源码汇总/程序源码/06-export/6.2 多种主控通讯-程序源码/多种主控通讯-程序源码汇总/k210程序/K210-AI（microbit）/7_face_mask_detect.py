import sensor, image, time, lcd
from maix import KPU

from modules import ybserial
import time

serial = ybserial()

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 1000)
clock = time.clock()

od_img = image.Image(size=(320,256), copy_to_fb=False)

anchor = (0.156250, 0.222548, 0.361328, 0.489583, 0.781250, 0.983133, 1.621094, 1.964286, 3.574219, 3.94000)
kpu = KPU()
print("ready load model")
kpu.load_kmodel("/sd/KPU/face_mask_detect/detect_5.kmodel")
kpu.init_yolo2(anchor, anchor_num=5, img_w=320, img_h=240, net_w=320 , net_h=256 ,layer_w=10 ,layer_h=8, threshold=0.7, nms_value=0.4, classes=2)

msg_=""
num=0
while True:
    clock.tick()
    img = sensor.snapshot()
    od_img.draw_image(img, 0,0)
    od_img.pix_to_ai()
    kpu.run_with_output(od_img)
    dect = kpu.regionlayer_yolo2()
    fps = clock.fps()
    if len(dect) > 0:
        print("dect:", dect)
        for l in dect :
            if l[4] :
                img.draw_rectangle(l[0],l[1],l[2],l[3], color=(0, 255, 0))
                img.draw_string(l[0],l[1]-24, "with mask", color=(0, 255, 0), scale=2)
                #msg_ = "Y"#带口罩
                num=1
            else:
                img.draw_rectangle(l[0],l[1],l[2],l[3], color=(255, 0, 0))
                img.draw_string(l[0],l[1]-24, "without mask", color=(255, 0, 0), scale=2)
                #msg_ = "N"#不带
                num=0

    if len(dect) > 0:
        send_data ="$"+"07"+str(num)+msg_+','+"#"
        time.sleep_ms(5)
        serial.send(send_data)

    a = img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 255), scale=2.0)
    lcd.display(img)
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
    lcd.display(img)

kpu.deinit()
