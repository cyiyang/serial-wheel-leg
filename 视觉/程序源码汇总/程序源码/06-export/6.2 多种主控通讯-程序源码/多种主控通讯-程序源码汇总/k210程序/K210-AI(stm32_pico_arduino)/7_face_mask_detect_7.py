import sensor, image, time, lcd
from maix import KPU

from modules import ybserial
import time
import binascii

serial = ybserial()
#字符串转10进制
def str_int(data_str):
    bb = binascii.hexlify(data_str)
    bb = str(bb)[2:-1]
    #print(bb)
    #print(type(bb))
    hex_1 = int(bb[0])*16
    hex_2 = int(bb[1],16)
    return hex_1+hex_2


def send_data(x,y,w,h,msg):
    start = 0x24
    end = 0x23
    length = 5
    class_num = 0x07 #例程编号
    class_group = 0xBB #例程组
    data_num = 0x00 #数据量
    fenge = 0x2c #逗号
    crc = 0 #校验位
    data = [] #数据组

    #参数都为0
    if x==0 and y==0 and w==0 and h ==0:
        pass
    else:
        #x(小端模式)
        low = x & 0xFF #低位
        high = x >> 8& 0xFF #高位
        data.append(low)
        data.append(fenge) #增加","
        data.append(high)
        data.append(fenge) #增加","

        #y(小端模式)
        low = y & 0xFF #低位
        high = y >> 8& 0xFF #高位
        data.append(low)
        data.append(fenge) #增加","
        data.append(high)
        data.append(fenge) #增加","

        #w(小端模式)
        low = w & 0xFF #低位
        high = w >> 8& 0xFF #高位
        data.append(low)
        data.append(fenge) #增加","
        data.append(high)
        data.append(fenge) #增加","

        #h(小端模式)
        low = h & 0xFF #低位
        high = h >> 8& 0xFF #高位
        data.append(low)
        data.append(fenge) #增加","
        data.append(high)
        data.append(fenge) #增加","

    if msg !=None:
        #msg
        for i in range(len(msg)):
            hec = str_int(msg[i])
            data.append(hec)
            data.append(fenge) #增加","
        #print(data)

    data_num = len(data)
    length += len(data)
    #print(length)

    send_merr = [length,class_num,class_group,data_num]
    for i in range(data_num):
        send_merr.append(data[i])
    #print(send_merr)

    #不加上CRC位，进行CRC运算
    for i in range(len(send_merr)):
        crc +=send_merr[i]
    crc = crc%256

    send_merr.insert(0,start) #插入头部
    send_merr.append(crc)
    send_merr.append(end)

    #print(send_merr)
    global send_buf
    send_buf = send_merr

send_buf = []
x_ = 0
y_ = 0
w_ = 0
h_ = 0
msg_ = ""

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
                x_ = l[0]
                y_ = l[1]
                w_ = l[2]
                h_ = l[3]
                msg_ = "Y"#带口罩
            else:
                img.draw_rectangle(l[0],l[1],l[2],l[3], color=(255, 0, 0))
                img.draw_string(l[0],l[1]-24, "without mask", color=(255, 0, 0), scale=2)
                x_ = l[0]
                y_ = l[1]
                w_ = l[2]
                h_ = l[3]
                msg_ = "N"#不带

    if len(dect) > 0:
        send_data(x_,y_,w_,h_,msg_)
        serial.send_bytearray(send_buf)
        print(send_buf)
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
    lcd.display(img)

kpu.deinit()
