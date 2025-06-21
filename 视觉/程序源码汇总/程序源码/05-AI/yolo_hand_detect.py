import lcd,sensor,image,gc
from maix import KPU
# 如果报Memory Error, 请尝试断开K210的连接后再插上重试
# If Memory Error is reported, try disconnecting K210 and then plugging in later
lcd.init()
sensor.reset()
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.RGB565)
#翻转屏幕
#sensor.set_vflip(True)

resize_img = image.Image(size=(320, 256))
anchor = (0.8125, 0.4556, 1.1328, 1.2667, 1.8594, 1.4889, 1.4844, 2.2000, 2.6484, 2.9333)
names = ['hand']

# 创建一个kpu对象 Init a KPU object
hand_detecter = KPU()
# 加载手部检测模型 load hand detect model
hand_detecter.load_kmodel("/sd/KPU/yolo_hand_detect/hand_detect.kmodel")
# 初始化YOLO2 init yolo2
# img_w,img_h为摄像头捕获到的图片原始大小, net_w和net_h为训练模型时使用的图片大小
# img_w and img_h represent the original dimensions of the image
hand_detecter.init_yolo2(anchor, anchor_num=len(anchor) // 2, img_w=320, img_h=240, net_w=320, net_h=256, layer_w=10, layer_h=8, threshold=0.68, nms_value=0.3, classes=len(names))

try:
    while True:
        gc.collect()
        img = sensor.snapshot()
        resize_img.draw_image(img, 0, 0).pix_to_ai()
        # 进行KPU运算
        hand_detecter.run_with_output(resize_img)
        # 进行YOLO2运算
        hands = hand_detecter.regionlayer_yolo2()
        for hand in hands:
            img.draw_rectangle(hand[0], hand[1], hand[2], hand[3], color=(0, 255, 0))
            img.draw_string(hand[0] + 2, hand[1] + 2, "%.2f" % (hand[5]), color=(0, 255, 0))
            img.draw_string(hand[0] + 2, hand[1] + 10, names[hand[4]], color=(0, 255, 0))
        lcd.display(img)
    hand_detecter.deinit()
    del hand_detecter
    del img
    gc.collect()
except Exception:
    hand_detecter.deinit()
    del hand_detecter
    del img
    gc.collect()

