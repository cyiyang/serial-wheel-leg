# !!!!!!由于车牌识别十分耗费内存，运行本案例请刷入Lite版固件!!!!!!
# !!!!!!This part is very memory-intensive, please burn canmv_yahboom_lite.bin first!!!!!!
import sensor, image, time, lcd, micropython,gc
from maix import KPU, utils
gc.collect();

# 初始化摄像头和LCD Initialize camera and LCD
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
# 如果报Sensor Timeout请尝试重新运行
sensor.skip_frames(time = 100)
clock = time.clock()
#utils.free_kpu_buffers()
# 加载UTF8字体
image.font_load(image.UTF8, 16, 16, "/sd/0xA00000.Dzk")
#image.font_load(image.UTF8, 16, 16, 0xA00000)

province = ("皖沪津渝冀晋蒙辽吉黑苏浙京闽赣鲁豫鄂湘粤桂琼川贵云藏陕甘青宁新")
ads = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
names = []
anchor = (8.30891522166988, 2.75630994889035, 5.18609903718768, 1.7863757404970702, 6.91480529053198, 3.825771881004435, 10.218567655549439, 3.69476690620971, 6.4088204258368195, 2.38813526350986)

lp_det_kpu = KPU()
lp_det_kpu.load_kmodel("/sd/KPU/car_licenseplate_recog/lp_detect.kmodel")
lp_det_kpu.init_yolo2(anchor, anchor_num=len(anchor) // 2, img_w=320, img_h=240, net_w=320, net_h=240, layer_w=20, layer_h=15, threshold=0.7, nms_value=0.3, classes=len(names))


lp_recog_kpu = KPU()
lp_recog_kpu.load_kmodel("/sd/KPU/car_licenseplate_recog/lp_recog.kmodel")

# load after-process data
lp_recog_kpu.lp_recog_load_weight_data("/sd/KPU/car_licenseplate_recog/lp_weight.bin")

def extend_box(x, y, w, h, scale):
    x1_t = x - scale*w
    x2_t = x + w + scale*w
    y1_t = y - scale*h
    y2_t = y + h + scale*h
    x1 = int(x1_t) if x1_t>1 else 1
    x2 = int(x2_t) if x2_t<320 else 319
    y1 = int(y1_t) if y1_t>1 else 1
    y2 = int(y2_t) if y2_t<256 else 255
    cut_img_w = x2-x1+1
    cut_img_h = y2-y1+1
    return x1, y1, cut_img_w, cut_img_h

lp_index_list = []
count = 0;

class LimitedQueue:
    def __init__(self, max_size=10):
        self.queue = []  # 初始化一个空列表作为队列
        self.max_size = max_size  # 设置最大长度
        self.frequency = {}  # 用于存储元素的频率
        self.mode = None  # 当前的众数
        self.max_count = 0  # 当前众数的最大频率

    def add(self, item):
        # 如果队列已满，删除第一个元素
        if len(self.queue) >= self.max_size:
            removed_item = self.queue.pop(0)

            # 更新频率字典
            if removed_item in self.frequency:
                self.frequency[removed_item] -= 1
                if self.frequency[removed_item] == 0:
                    del self.frequency[removed_item]

        # 添加新元素
        self.queue.append(item)

        # 更新频率字典
        if item in self.frequency:
            self.frequency[item] += 1
        else:
            self.frequency[item] = 1

        # 更新众数的逻辑
        self.update_mode(item)

    def update_mode(self, item):
        # 更新众数逻辑
        if self.frequency[item] > self.max_count:
            self.max_count = self.frequency[item]
            self.mode = item
        elif self.frequency[item] == self.max_count:
            # 处理重复的众数
            if self.mode is None or (item != -1 and item < self.mode):  # 避免将 -1 作为众数
                self.mode = item

        # 如果当前众数的计数低于最大计数，重新计算众数
        if self.mode is not None and self.frequency[self.mode] < self.max_count:
            self.reset_mode()

    def reset_mode(self):
        # 重新计算众数
        max_freq = 0
        new_mode = None
        for item, freq in self.frequency.items():
            if freq > max_freq and item != -1:  # 确保忽略 -1
                max_freq = freq
                new_mode = item

        self.max_count = max_freq
        self.mode = new_mode

    def get_mode(self):
        return self.mode if self.mode is not None else -1  # 返回当前众数，确保不会返回 None

    def __str__(self):
        return str(self.queue)  # 可以方便地打印队列中的元素

last_ten = LimitedQueue();

try:
    while 1:
        gc.collect()
        clock.tick()                    # Update the FPS clock.
        img = sensor.snapshot()
        lp_det_kpu.run_with_output(img)
        lps = lp_det_kpu.regionlayer_yolo2()
        if len(lps)<=0:
            last_ten.add(-1);
        for lp in lps:
            # 框出车牌位置
            x, y, w, h = extend_box(lp[0], lp[1], lp[2], lp[3], 0.08)
            img.draw_rectangle(x, y, w, h, color=(0, 255, 0))
            # 识别车牌
            lp = []
            lp_img = img.cut(x, y, w, h)
            resize_img = lp_img.resize(208, 64)
            resize_img.replace(hmirror=True)
            resize_img.pix_to_ai()
            lp_recog_kpu.run_with_output(resize_img)
            output = lp_recog_kpu.lp_recog()
            for o in output:
                lp.append(o.index(max(o)))

            last_ten.add(lp[0]);
            print(last_ten);
            lp[0] = last_ten.mode;
            #print(lp[0])
            #if lp[0] == -1:
                #del lp
                #del lp_img
                #del resize_img
                #continue;
            show_lp_str = "%s  %s-%s%s%s%s%s" %(province[lp[0]], ads[lp[1]],ads[lp[2]], ads[lp[3]], ads[lp[4]], ads[lp[5]], ads[lp[6]])
            # 此处需要重新加载字体文件, 否则中文显示会异常
            image.font_load(image.UTF8, 16, 16, "/sd/0xA00000.Dzk")
            img.draw_string(x + 2, y - 20 - 2, "%s"%show_lp_str,color=(255, 0, 0),x_spacing=2, mono_space=False,scale=2)
            del lp
            del lp_img
            del resize_img
            gc.collect()
        #print(last_ten);
        lcd.display(img)
except Exception as e:
    print(e)
    lp_det_kpu.deinit()
    lp_recog_kpu.deinit()
    del lp_det_kpu
    del lp_recog_kpu
    gc.collect()
