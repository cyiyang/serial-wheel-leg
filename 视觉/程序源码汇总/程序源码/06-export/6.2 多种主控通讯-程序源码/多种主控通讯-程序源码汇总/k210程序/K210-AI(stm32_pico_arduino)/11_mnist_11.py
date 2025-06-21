import sensor, image, time, lcd
from maix import KPU
import gc


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
    class_num = 0x0B #例程编号
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
#x_ = 0
#y_ = 0
#w_ = 0
#h_ = 0
msg_ = ""

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.skip_frames(time = 100)
clock = time.clock()

kpu = KPU()
kpu.load_kmodel("/sd/KPU/mnist/uint8_mnist_cnn_model.kmodel")


while True:
    gc.collect()
    img = sensor.snapshot()
    img_mnist1=img.to_grayscale(1)
    img_mnist2=img_mnist1.resize(112,112)
    img_mnist2.invert()
    img_mnist2.strech_char(1)
    img_mnist2.pix_to_ai()

    out = kpu.run_with_output(img_mnist2, getlist=True)
    max_mnist = max(out)
    index_mnist = out.index(max_mnist)
    msg_ = str(index_mnist)
    send_data(0,0,0,0,msg_)#封装
    serial.send_bytearray(send_buf)#发送
    score = KPU.sigmoid(max_mnist)
    if index_mnist == 1:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    elif index_mnist == 5:
        if score > 0.999:
            display_str = "num: %d" % index_mnist
            print(display_str, score)
            img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    else:
        display_str = "num: %d" % index_mnist
        print(display_str, score)
        img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
    lcd.display(img)


kpu.deinit()
