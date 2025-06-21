import sensor, image, time, lcd
from maix import KPU
from modules import ybserial
from robot_Lib import Robot

ser = ybserial()
bot = Robot(ser)
bot.set_beep(50)
bot.set_colorful_lamps(0xff, 0, 0, 0)


lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.skip_frames(time=100)
clock = time.clock()

kpu = KPU()
kpu.load_kmodel("/sd/KPU/mnist/uint8_mnist_cnn_model.kmodel")


def get_result_filt(index_mnist, score):
    result_str = ''
    if index_mnist == 1: #白底
        if score > 0.999:
            result_str = "num: %d" % index_mnist
    elif index_mnist == 5: #黑底
        if score > 0.9995:
            result_str = "num: %d" % index_mnist
    else:
        result_str = "num: %d" % index_mnist
    return result_str

last_number = 0
count_number = 0
count_clear = 0
MAX_COUNT = 3

while True:
    img = sensor.snapshot()
    img_mnist1=img.to_grayscale(1)
    img_mnist2=img_mnist1.resize(112,112)
    img_mnist2.invert()
    img_mnist2.strech_char(1)
    img_mnist2.pix_to_ai()

    out = kpu.run_with_output(img_mnist2, getlist=True)
    max_mnist = max(out)
    index_mnist = out.index(max_mnist)
    score = KPU.sigmoid(max_mnist)
    display_str = get_result_filt(index_mnist, score)
    if len(display_str) > 0:
        img.draw_string(4,3,display_str,color=(0,0,0),scale=2)
        print(display_str)
        count_clear = 0
        if last_number != index_mnist:
            last_number = index_mnist
            count_number = 0
        else:
            count_number = count_number + 1
        if count_number > MAX_COUNT:
            if index_mnist == 0:
                bot.set_colorful_lamps(0xff, 0, 255, 0)
            else:
                bot.set_colorful_lamps(index_mnist, 0, 255, 0, 1)
    else:
        count_clear = count_clear + 1
        if count_clear > MAX_COUNT:
            if count_clear == MAX_COUNT + 1:
                bot.set_colorful_lamps(0xff, 0, 0, 0)
    lcd.display(img)

kpu.deinit()
