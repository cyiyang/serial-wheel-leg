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
    class_num = 0x06 #例程编号
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
sensor.skip_frames(time = 100)
clock = time.clock()

anchor = (0.1075, 0.126875, 0.126875, 0.175, 0.1465625, 0.2246875, 0.1953125, 0.25375, 0.2440625, 0.351875, 0.341875, 0.4721875, 0.5078125, 0.6696875, 0.8984375, 1.099687, 2.129062, 2.425937)
kpu = KPU()
kpu.load_kmodel("/sd/KPU/yolo_face_detect/face_detect_320x240.kmodel")
kpu.init_yolo2(anchor, anchor_num=9, img_w=320, img_h=240, net_w=320 , net_h=240 ,layer_w=10 ,layer_h=8, threshold=0.7, nms_value=0.2, classes=1)

lm68_kpu = KPU()
print("ready load model")
lm68_kpu.load_kmodel("/sd/KPU/face_detect_with_68landmark/landmark68.kmodel")


def extend_box(x, y, w, h, scale):
    x1_t = x - scale*w
    x2_t = x + w + scale*w
    y1_t = y - scale*h
    y2_t = y + h + scale*h
    x1 = int(x1_t) if x1_t>1 else 1
    x2 = int(x2_t) if x2_t<320 else 319
    y1 = int(y1_t) if y1_t>1 else 1
    y2 = int(y2_t) if y2_t<240 else 239
    cut_img_w = x2-x1+1
    cut_img_h = y2-y1+1
    return x1, y1, cut_img_w, cut_img_h

while True:
    clock.tick()
    img = sensor.snapshot()
    kpu.run_with_output(img)
    dect = kpu.regionlayer_yolo2()
    fps = clock.fps()
    if len(dect) > 0:
        #print("dect:",dect)
        for l in dect :
            x1, y1, cut_img_w, cut_img_h = extend_box(l[0], l[1], l[2], l[3], scale=0.08)
            face_cut = img.cut(x1, y1, cut_img_w, cut_img_h)
            a = img.draw_rectangle(l[0],l[1],l[2],l[3], color=(0, 255, 0))
            face_cut_128 = face_cut.resize(128, 128)
            face_cut_128.pix_to_ai()
            out = lm68_kpu.run_with_output(face_cut_128, getlist=True)
            x_ = l[0]
            y_ = l[1]
            w_ = l[2]
            h_ = l[3]
            #print("out:",len(out))
            for j in range(68):
                x = int(KPU.sigmoid(out[2 * j])*cut_img_w + x1)
                y = int(KPU.sigmoid(out[2 * j + 1])*cut_img_h + y1)
                #a = img.draw_cross(x, y, size=1, color=(0, 0, 255))
                a = img.draw_circle(x, y, 2, color=(0, 0, 255), fill=True)
            del (face_cut_128)
            del (face_cut)

    if len(dect) > 0:
        send_data(x_,y_,w_,h_,None)
        serial.send_bytearray(send_buf)
        print(send_buf)

    a = img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 255), scale=2.0)
    lcd.display(img)

kpu.deinit()
lm68_kpu.deinit()


