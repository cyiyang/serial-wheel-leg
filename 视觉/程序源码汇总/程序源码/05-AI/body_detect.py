import sensor, image, time, lcd
from maix import KPU
import gc

# 初始化摄像头和LCD Initialize camera and LCD
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 1000)
# 记录FPS (Record FPS)
clock = time.clock()
od_img = image.Image(size=(320,256))

names = ['body']
anchor = (0.0978, 0.1758, 0.1842, 0.3834, 0.3532, 0.5982, 0.4855, 1.1146, 0.8869, 1.6407, 1.2388, 3.4157, 2.0942, 2.1114, 2.7138, 5.0008, 6.0293, 6.4540)
body_kpu = KPU()

# 加载YOLO模型 Load YOLO model
body_kpu.load_kmodel("/sd/KPU/head_body_detect/person_detect_v1.kmodel")
body_kpu.init_yolo2(anchor, anchor_num=len(anchor) // 2, img_w=320, img_h=240, net_w=320, net_h=256, layer_w=10, layer_h=8, threshold=0.5, nms_value=0.2, classes=len(names))

try:
    while True:
        # 手动垃圾回收,避免内存泄漏 Manual garbage collection to avoid memory leaks
        gc.collect()
        clock.tick()
        img = sensor.snapshot()
        a = od_img.draw_image(img, 0,0)
        od_img.pix_to_ai()
        body_kpu.run_with_output(input=od_img, getlist=False, get_feature=False)
        body_boxes = body_kpu.regionlayer_yolo2()
        if len(body_boxes) > 0:
            for l in body_boxes :
                a = img.draw_rectangle(l[0],l[1],l[2],l[3], color=(255, 0, 0))

        fps = clock.fps()
        a = img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
        lcd.display(img)
except Exception as e:
    print(e)
    body_kpu.deinit()
    del body_kpu
    gc.collect()
