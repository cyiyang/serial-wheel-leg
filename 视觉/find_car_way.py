# find_car_way - By: 陈亦杨 - 周三 4月 9 2025
import sensor, image, time, lcd
from fpioa_manager import fm
from modules import ybrgb
from machine import UART
import random
import ubinascii
import time

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) #灰度
sensor.set_framesize(sensor.QVGA) #320×240
sensor.skip_frames(time = 3000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
lcd.init()

RGB = ybrgb()
clock = time.clock()

#引脚映射
fm.register(6, fm.fpioa.UART2_RX,force = True)
fm.register(8, fm.fpioa.UART2_TX,force = True)
yb_uart = UART(UART.UART2, 115200, 8, 0, 1, timeout=1000, read_buf_len=4096)

head_flag = 0xA5
end_flag = 0xB6

#加上阈值和检测中心
green_threshold = ((0,50)) #黑线
roi1 = [0,80,320,16]  #巡线区
roi2 = [0,180,320,16]   #关键点巡线区
expectedValue = 160
err = 0
last_err = 0
Kp = 1
Kd = 0.001
speed = 0
speed_right = 0
speed_left = 0
Flag = 0

#def send_data(way_left,way_right,Flag):
    ##编写头报文和尾报文
    #code_way_left = int(hex(way_left))
    #code_way_right = int(hex(way_right))
    #code_flag = int(hex(Flag))
    #send_data = [head_flag,code_way_left,code_way_right,code_flag,end_flag]
    #serial.send_bytearray(send_data)
    #print(send_data)
def send_data(way_left,way_right,Flag):
    #不足三位数前面补零处理
    way_left = way_left + 160
    Left = "{:03d}".format(way_left)
    data = ':' + Left + 'f' + str(Flag) + '@'
    yb_uart.write(data)
    print(data)

def find_max(blobs): #最大矩形块
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob


def find_way(img): #找偏差
    global speed , speed_right , speed_left , Flag
    rect1 = img.find_blobs([green_threshold],roi = roi1,area_threshold = 200,merge = True)
    rect2 = img.find_blobs([green_threshold],roi = roi2,area_threshold = 200,merge = True,margin = 120)
    if rect1:
        rect1_max = find_max(rect1)
        temp = img.draw_rectangle(rect1_max[0:4])
        temp = img.draw_cross(rect1_max[5],rect1_max[6])
        actualValue = rect1_max[5]
        x_error = actualValue-expectedValue
        speed_left , speed_right = calc_pid(x_error)
    if rect2:
        rect2_max = find_max(rect2)
        temp = img.draw_rectangle(rect2_max[0:4])
        temp = img.draw_cross(rect2_max[5],rect2_max[6])
        if rect2_max[2] >= 100:
            Flag = 1
        else:
            Flag = 0


def calc_pid(x_error):   #PID计算
    global err , last_err
    err = x_error
    speed_left = speed - err*Kp
    speed_right = speed + err*Kp
    last_err = err
    return speed_left,speed_right


while(True):
    img = sensor.snapshot()
    RGB.set(1,1,1)
    find_way(img)
    send_data(speed_left,speed_right,Flag)
    lcd.display(img)



