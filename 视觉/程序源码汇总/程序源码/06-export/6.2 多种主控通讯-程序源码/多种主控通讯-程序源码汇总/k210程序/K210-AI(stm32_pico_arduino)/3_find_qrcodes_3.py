import sensor, image, time, lcd


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
    class_num = 0x03 #例程编号
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
while(True):
    clock.tick()
    img = sensor.snapshot()
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = 127, thickness=3)
        img.draw_string(code.x(),code.y()-20,code.payload(),color=(255,0,0),scale=2)
        #print(code)
        x_ = code.x()
        y_ = code.y()
        w_ = code.w()
        h_ = code.h()
        msg_ = code.payload()
       #print(code.payload())

    if msg_ != "":
        send_data(x_,y_,w_,h_,msg_)
        msg_ = ""
        serial.send_bytearray(send_buf)
        print(send_buf)
    lcd.display(img)
    #print(clock.fps())


