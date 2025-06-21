import lvgl as lv
import lvgl_helper as lv_h
import lodepng as png
import lcd
import time
import ustruct as struct
from machine import Timer
import touchscreen as ts


lcd.init()
ts.init()
lv.init()

disp_buf1 = lv.disp_buf_t()
buf1_1 = bytearray(320*10)
lv.disp_buf_init(disp_buf1,buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
lv.disp_drv_init(disp_drv)
disp_drv.buffer = disp_buf1
disp_drv.flush_cb = lv_h.flush
disp_drv.hor_res = 320
disp_drv.ver_res = 240
lv.disp_drv_register(disp_drv)


indev_drv = lv.indev_drv_t()
lv.indev_drv_init(indev_drv)
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = lv_h.read
lv.indev_drv_register(indev_drv)

# lv.log_register_print_cb(lv_h.log)
lv.log_register_print_cb(lambda level,path,line,msg: print('%s(%d): %s' % (path, line, msg)))


class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)


def get_png_info(decoder, src, header):
    # Only handle variable image types
    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV
    png_header = bytes(lv.img_dsc_t.cast(src).data.__dereference__(24))
    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = struct.unpack(">LL", png_header[start:start+8])
        except struct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV
    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA
    return lv.RES.OK

# Convert color formats
#def convert_rgba8888_to_bgra5658(img_view):
    #img_size = int(len(img_view)) // 4
    #p = img_view
    #j = 0
    #for i in range(img_size):
        #r = p[i*4]
        #g = p[i*4 + 1]
        #b = p[i*4 + 2]
        #a = p[i*4 + 3]
        #j = i*3
        #p[j] = \
            #((g & 0b11111000) >> 3) |\
            #((r & 0b00011100) << 3)
        #p[j + 1] = \
            #((r & 0b11100000) >> 5) |\
            #((b & 0b11111000) )
        #p[j + 2] = a
def convert_rgba8888_to_bgra5658(img_view):
    img_size = int(len(img_view)) // 4
    p = img_view
    j = 0
    for i in range(img_size):
        g = p[i*4]
        b = p[i*4 + 1]
        r = p[i*4 + 2]
        a = p[i*4 + 3]
        j = i*3
        p[j] = \
            ((b & 0b11111000) >> 3) |\
            ((g & 0b00011100) << 3)
        p[j + 1] = \
            ((g & 0b11100000) >> 5) |\
            ((r & 0b11111000) )
        p[j + 2] = a

# Read and parse PNG file
def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.cast(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = int(png_width.int_val * png_height.int_val * 4)
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)
    #convert_rgba8888_to_bgra5658(img_view)
    lv_h.rgba8888_to_5658(img_view)
    dsc.img_data = img_data
    return lv.RES.OK

# Register new image decoder
decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

#Create a screen with a draggable image
#with open('/flash/png_decoder_test.png','rb') as f:
with open('/sd/logo_1.png','rb') as f:
  png_data = f.read()

png_img_dsc = lv.img_dsc_t({
    'data_size': len(png_data),
    'data': png_data
})

scr = lv.obj()

# Create an image on the left using the decoder
lv.img.cache_set_size(2)
img1 = lv.img(scr)
img1.align(scr, lv.ALIGN.CENTER, 0, 0)
img1.set_src(png_img_dsc)
img1.set_drag(True)

label_style = lv.style_t(lv.style_plain)
label_style.text.font = lv.font_roboto_28

label1 = lv.label(scr)
label1.align(scr, lv.ALIGN.IN_BOTTOM_MID, -40, -20)
label1.set_style(lv.label.STYLE.MAIN, label_style)
label1.set_size(50, 30)
label1.set_text("")


# Load the screen and display image
lv.scr_load(scr)

#def on_timer(timer):
    #lv.tick_inc(5)

#timer = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PERIODIC, period=5, unit=Timer.UNIT_MS, callback=on_timer, arg=None)
#try:
    #while True:
        #tim = time.ticks_ms()
        #lv.task_handler()
        #while time.ticks_ms()-tim < 5:
            #pass
#except Exception as e:
    #print(e)
    #timer.deinit()
    #del timer

status_last = ts.STATUS_IDLE
x_last = 0
y_last = 0

tim = time.ticks_ms()
while True:
    if time.ticks_ms()-tim > 5:
        tim = time.ticks_ms()
        (status, x, y) = ts.read()
        if status == ts.STATUS_RELEASE:
            label1.set_text("")
        else:
            label1.set_text("(%3d, %3d)" % (x, y))
        lv.task_handler()
        lv.tick_inc(5)
