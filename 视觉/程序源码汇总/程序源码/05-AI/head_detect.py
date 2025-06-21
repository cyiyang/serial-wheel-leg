import sensor, image, time, lcd
from maix import KPU
import gc

# 初始化摄像头和LCD Initialize camera and LCD
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 1000)
# 记录FPS Record FPS
clock = time.clock()
od_img = image.Image(size=(320,256))

names = ['head']
anchor = (0.1074, 0.1458, 0.1367, 0.2137, 0.1758, 0.2824, 0.2441, 0.3333, 0.2188, 0.4167, 0.2969, 0.5000, 0.4102, 0.6667, 0.6094, 0.9722, 1.2364, 1.6915)
head_kpu = KPU()

# 加载YOLO模型 Load YOLO model
head_kpu.load_kmodel("/sd/KPU/head_body_detect/head_detect_v1.kmodel")
head_kpu.init_yolo2(anchor, anchor_num=len(anchor) // 2, img_w=320, img_h=240, net_w=320, net_h=256, layer_w=10, layer_h=8, threshold=0.5, nms_value=0.2, classes=len(names))

try:
    while True:
        # 手动垃圾回收,避免内存泄漏 Manual garbage collection to avoid memory leaks
        gc.collect()
        clock.tick()
        # 捕获图像 Capture image
        img = sensor.snapshot()
        a = od_img.draw_image(img, 0, 0)
        # 准备图像用于AI处理 Prepare image for AI processing
        od_img.pix_to_ai()
        head_kpu.run_with_output(input=od_img, getlist=False, get_feature=False)
        # 获取YOLO输出的边界框 Get YOLO output bounding boxes
        head_boxes = head_kpu.regionlayer_yolo2()

        # 绘制捕获到的人脸 Draw detected faces
        if len(head_boxes) > 0:
            for l in head_boxes:
                a = img.draw_rectangle(l[0], l[1], l[2], l[3], color=(0, 255, 0))
        fps = clock.fps()
        a = img.draw_string(0, 0, "%2.1ffps" % (fps), color=(0, 60, 128), scale=2.0)
        lcd.display(img)
except Exception as e:
    head_kpu.deinit()
    del head_kpu
    gc.collect()
