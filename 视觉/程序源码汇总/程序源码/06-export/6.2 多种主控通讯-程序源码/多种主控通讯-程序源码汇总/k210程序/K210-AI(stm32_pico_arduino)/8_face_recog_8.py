import sensor, image, time, lcd
import gc
from maix import KPU
from maix import GPIO, utils
from fpioa_manager import fm
from board import board_info

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
    class_num = 0x08 #例程编号
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

feature_img = image.Image(size=(64,64), copy_to_fb=False)
feature_img.pix_to_ai()

FACE_PIC_SIZE = 64
dst_point =[(int(38.2946 * FACE_PIC_SIZE / 112), int(51.6963 * FACE_PIC_SIZE / 112)),
            (int(73.5318 * FACE_PIC_SIZE / 112), int(51.5014 * FACE_PIC_SIZE / 112)),
            (int(56.0252 * FACE_PIC_SIZE / 112), int(71.7366 * FACE_PIC_SIZE / 112)),
            (int(41.5493 * FACE_PIC_SIZE / 112), int(92.3655 * FACE_PIC_SIZE / 112)),
            (int(70.7299 * FACE_PIC_SIZE / 112), int(92.2041 * FACE_PIC_SIZE / 112)) ]

anchor = (0.1075, 0.126875, 0.126875, 0.175, 0.1465625, 0.2246875, 0.1953125, 0.25375, 0.2440625, 0.351875, 0.341875, 0.4721875, 0.5078125, 0.6696875, 0.8984375, 1.099687, 2.129062, 2.425937)
kpu = KPU()
kpu.load_kmodel("/sd/KPU/yolo_face_detect/face_detect_320x240.kmodel")
kpu.init_yolo2(anchor, anchor_num=9, img_w=320, img_h=240, net_w=320 , net_h=240 ,layer_w=10 ,layer_h=8, threshold=0.7, nms_value=0.2, classes=1)

ld5_kpu = KPU()
print("ready load model")
ld5_kpu.load_kmodel("/sd/KPU/face_recognization/ld5.kmodel")

fea_kpu = KPU()
print("ready load model")
fea_kpu.load_kmodel("/sd/KPU/face_recognization/feature_extraction.kmodel")

start_processing = False
BOUNCE_PROTECTION = 50

fm.register(board_info.BOOT_KEY, fm.fpioa.GPIOHS0)
key_gpio = GPIO(GPIO.GPIOHS0, GPIO.IN)
def set_key_state(*_):
    global start_processing
    start_processing = True
    time.sleep_ms(BOUNCE_PROTECTION)
key_gpio.irq(set_key_state, GPIO.IRQ_RISING, GPIO.WAKEUP_NOT_SUPPORT)


record_ftrs = []
THRESHOLD = 80.5
recog_flag = False

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
    gc.collect()
    # print("mem free:",gc.mem_free())
    # print("heap free:",utils.heap_free())
    clock.tick()
    img = sensor.snapshot()
    kpu.run_with_output(img)
    dect = kpu.regionlayer_yolo2()
    fps = clock.fps()
    if len(dect) > 0:
        for l in dect :
            x1, y1, cut_img_w, cut_img_h= extend_box(l[0], l[1], l[2], l[3], scale=0)
            face_cut = img.cut(x1, y1, cut_img_w, cut_img_h)
            face_cut_128 = face_cut.resize(128, 128)
            face_cut_128.pix_to_ai()
            out = ld5_kpu.run_with_output(face_cut_128, getlist=True)
            face_key_point = []
            for j in range(5):
                x = int(KPU.sigmoid(out[2 * j])*cut_img_w + x1)
                y = int(KPU.sigmoid(out[2 * j + 1])*cut_img_h + y1)
                face_key_point.append((x,y))
            T = image.get_affine_transform(face_key_point, dst_point)
            image.warp_affine_ai(img, feature_img, T)
            feature = fea_kpu.run_with_output(feature_img, get_feature = True)
            del face_key_point
            scores = []
            for j in range(len(record_ftrs)):
                score = kpu.feature_compare(record_ftrs[j], feature)
                scores.append(score)
            if len(scores):
                max_score = max(scores)
                index = scores.index(max_score)
                if max_score > THRESHOLD:
                    img.draw_string(0, 195, "persion:%d,score:%2.1f" %(index, max_score), color=(0, 255, 0), scale=2)
                    recog_flag = True
                    # if index == 0:
                    #     img.draw_string(0, 195, "zhangsan, score:%2.1f" %(max_score), color=(0, 255, 0), scale=2)
                    # elif index == 1:
                    #     img.draw_string(0, 195, "lisi, score:%2.1f" %(max_score), color=(0, 255, 0), scale=2)
                    # elif index == 2:
                    #     img.draw_string(0, 195, "wangwu, score:%2.1f" %(max_score), color=(0, 255, 0), scale=2)
                    # elif index == 3:
                    #     img.draw_string(0, 195, "zhaoliu, score:%2.1f" %(max_score), color=(0, 255, 0), scale=2)
                    # else:
                    #     img.draw_string(0, 195, "persion:%d,score:%2.1f" %(index, max_score), color=(0, 255, 0), scale=2)
                else:
                    img.draw_string(0, 195, "unregistered,score:%2.1f" %(max_score), color=(255, 0, 0), scale=2)
            del scores
            if start_processing:
                record_ftrs.append(feature)
                print("record_ftrs:%d" % len(record_ftrs))
                start_processing = False

            if recog_flag:
                img.draw_rectangle(l[0],l[1],l[2],l[3], color=(0, 255, 0))
                recog_flag = False
                x_ = l[0]
                y_ = l[1]
                w_ = l[2]
                h_ = l[3]
                msg_ = "Y"
            else:
                img.draw_rectangle(l[0],l[1],l[2],l[3], color=(255, 255, 255))
                x_ = l[0]
                y_ = l[1]
                w_ = l[2]
                h_ = l[3]
                msg_ = "N"
            del (face_cut_128)
            del (face_cut)
    if len(dect) > 0:
        send_data(x_,y_,w_,h_,msg_)
        serial.send_bytearray(send_buf)
        print(send_buf)
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 255), scale=2.0)
    img.draw_string(0, 215, "press boot key to regist face", color=(255, 100, 0), scale=2.0)
    lcd.display(img)


kpu.deinit()
ld5_kpu.deinit()
fea_kpu.deinit()
