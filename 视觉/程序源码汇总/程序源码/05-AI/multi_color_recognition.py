'''
@file:    multi_color_recognition.py
@company: yahboom
@description: 多种颜色识别
'''


import sensor
import image
import time
import lcd

#初始化LCD
lcd.init()
#以下是初始化传感器
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
#帧率时钟
clock = time.clock()

#方框参数
r = [(320//2)-(50//2), (240//2)-(50//2), 50, 50]


# 储存多种颜色的LAB阈值
color_thresholds = [
    (31, 69, 27, 58, 14, 36),# Red
    #(20,80,20,62,20,35),


    #(49, 74, -26, 48, 15, 55),# Green
    (14, 61, -39, -6, 0, 14),

    (14, 66, 1, 38, -56, -12),# Blue
    #(21,50,-7,8,-35,-11),

    (49, 77, -8, 52, 16, 60),# Yellow
    #(65, 78, -10, -5, 38, 50),
]

color_strings = ['Red', 'Green', 'Blue', 'Yellow']

print("Start Color Recognition...")

#loop
while True:
    #用于计算帧率的函数，这里表示开始计时
    clock.tick()
    #从传感器捕获一张图像
    img = sensor.snapshot()
    #遍历多种颜色
    for color_idx, threshold in enumerate(color_thresholds):
        #遍历图像中找到的颜色区块
        blobs = img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10)
        #是否找到
        if blobs:
            #画方框
            for blob in blobs:
                color_new = (255 , 255,255)
                if color_idx == 0 :
                    color_new = (255 , 0,0)
                elif color_idx == 1:
                    color_new = ( 0,255 ,0)
                elif color_idx == 2:
                    color_new = ( 0,0,255)
                elif color_idx == 3:
                    color_new = ( 255,255,0)
                img.draw_rectangle(blob.rect(), color=color_new,thickness = 3)
                img.draw_cross(blob.cx(), blob.cy(), color=color_new)
                img.draw_string(blob.cx() + 10, blob.cy() - 10, color_strings[color_idx], color=color_new)
    #显示在LCD上
    lcd.display(img)
    #打印帧率
    print(clock.fps())
