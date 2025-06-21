from machine import UART, Pin, Timer
import utime
import binascii
#cc=chr(65) #10进制转成ascii

###########处理信息用到的间接变量########
rxx = bytes()
data_buf_str = []#字符串型列表
data_buf_int = []#整型列表

data_data = []#数据量中的有效数据
data_len = 0 #包的长度

#字符串转10进制
def str_int(data_str):
    data_strr = data_str
    hex_1 = int(data_strr[0],16)*16
    hex_2 = int(data_strr[1],16)
    return hex_1+hex_2

def deal_data():
    global data_buf_int,data_len,data_buf_str,data_data
    
    duo_flag = 0 #逗号标志位 
    start = chr(data_buf_int[0]) #包头
    end = chr(data_buf_int[-1])  #包尾
    #print(start,end)
    
    #校验包头包尾
    if(start != '$') and (end != '#'):
        return
    
    
    lenght = data_buf_int[1] #长度
    egnum = data_buf_int[2] #例程编号
    #num_group = data_buf_int[3] #例程组
    data_num = data_buf_int[4] #数据量
    crc = 0 #校验码
    
    
    for i in range(len(data_buf_int)):
        crc = crc + data_buf_int[i]
    crc = crc - data_buf_int[0]-data_buf_int[-1]-data_buf_int[-2] #减掉包头、包尾和检验码
    crc = crc % 256
    data_len = len(data_buf_int)-2
    #校验码不对或者长度不对
    if crc != data_buf_int[-2] or data_len != lenght:
        data_len = 0
        data_buf_str = []#字符串型列表
        data_buf_int = []#整型列表
        data_data = []#数据量中的有效数据
        return
    
    #print (data_num)
    for i in range(0,data_num):
        #去掉逗号的，但不可能连续
        if data_buf_int[5+i] == 44 and duo_flag == 0:
            duo_flag = 1
            continue
        data_data.append(data_buf_int[5+i])
        duo_flag = 0
        
    print("import data:",data_data)
    get_data(egnum)
    
    
#传入的是例程编号
def get_data(eg_num):
    global data_buf_int,data_len,data_buf_str,data_data
    msg = ""
    x = 0
    y = 0
    w = 0
    h = 0
    iid = 0 
    if eg_num == 1 or eg_num ==5 or eg_num ==6 :
       x=data_data[1]<<8 | data_data[0]
       y=data_data[3]<<8 | data_data[2]
       w=data_data[5]<<8 | data_data[4]
       h=data_data[7]<<8 | data_data[6]
    
       
    elif eg_num == 2 or eg_num == 3:
       x=data_data[1]<<8 | data_data[0]
       y=data_data[3]<<8 | data_data[2]
       w=data_data[5]<<8 | data_data[4]
       h=data_data[7]<<8 | data_data[6]
       for i in range(8,len(data_data)):
           msg +=chr(data_data[i]) #转成ascii码

    
    elif eg_num == 4:
       x=data_data[1]<<8 | data_data[0]
       y=data_data[3]<<8 | data_data[2]
       w=data_data[5]<<8 | data_data[4]
       h=data_data[7]<<8 | data_data[6]
       idd ="id="+chr(data_data[8])+chr(data_data[9]);
       for i in range(10,len(data_data)):
           msg +=chr(data_data[i]) #转成ascii码
           
    elif eg_num == 7 or eg_num == 8:
       x=data_data[1]<<8 | data_data[0]
       y=data_data[3]<<8 | data_data[2]
       w=data_data[5]<<8 | data_data[4]
       h=data_data[7]<<8 | data_data[6]
       idd =chr(data[8])

        
    elif eg_num == 9:
       x=data_data[1]<<8 | data_data[0]
       y=data_data[3]<<8 | data_data[2]
       w=data_data[5]<<8 | data_data[4]
       h=data_data[7]<<8 | data_data[6]
       for i in range(8,len(data_data)):
           msg +=chr(data_data[i]) #转成ascii码

           
    elif eg_num == 10 or eg_num == 11:
        idd = chr(data_data[0])

    
    #清空数据
    data_len = 0
    data_buf_str = []#字符串型列表
    data_buf_int = []#整型列表
    data_data = []#数据量中的有效数据
    
    #赋值信息
    global k210_data_x,k210_data_y,k210_data_w,k210_data_h,k210_data_id,k210_data_msg,k210_data_class
    k210_data_x = x     #横坐标
    k210_data_y = y     #纵坐标
    k210_data_w = w     #宽度
    k210_data_h = h     #长度
    k210_data_id = iid    #标签
    k210_data_msg = msg  #信息
    k210_data_class = eg_num #例程编号
        
        
def recv_k210_data(rxx_c):
    global data_buf_str,data_buf_int
    data_one = binascii.hexlify(rxx_c) #转16进制形式的字符串
    data_one = data_one.decode('utf-8') #去掉b''
    #print(data_one)
    #每两个字符做切割
    for i in range(0,len(data_one),2):
    #print(data_one[i:i+2])
        data_buf_str.append(data_one[i:i+2])    
    #print(data_buf)
    for i in range(len(data_buf_str)):
        hint = str_int(data_buf_str[i]) 
        data_buf_int.append(hint)
    #print(data_buf_int)
    deal_data()    
        
##############################################**main**############################################

u1 = UART(1, baudrate=115200, tx=Pin(8), rx=Pin(9), bits=8, parity=None, stop=0)  # 设置波特率和串口号

k210_data_class = 0     #例程编号
k210_data_x = 0     	#横坐标
k210_data_y = 0     	#纵坐标
k210_data_w = 0     	#宽度
k210_data_h = 0     	#长度
k210_data_id = 0    	#标签
k210_data_msg = ""  	#信息


while True:
    while u1.any() >0:
        rxx=u1.read()
        #u1.write(rxx)
        recv_k210_data(rxx) #处理收到的k210信息
    
    if k210_data_class != 0:
        #是条形码的例程
        if k210_data_class == 2:
            sstr = "x="+str(k210_data_x)  +" y="+str(k210_data_y)  +" w="+str(k210_data_w)  +" h="+str(k210_data_h) +"\r\n"
            u1.write(sstr)
            u1.write(k210_data_msg+'\r\n')
        k210_data_class = 0
