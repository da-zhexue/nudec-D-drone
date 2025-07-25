import sensor, time
from machine import UART

# 定义红色的颜色阈值范围
red = (0, 55, -19, 14, -27, 18)
# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 设置像素格式为 RGB565
sensor.set_framesize(sensor.QQVGA)  # 设置帧大小为 QQVGA
sensor.skip_frames(30)  # 跳过前 30 帧以稳定摄像头
sensor.set_auto_gain(False)  # 关闭自动增益以防止图像过曝
sensor.set_auto_whitebal(False)  # 关闭自动白平衡

# 初始化时钟和 UART 通信
clock = time.clock()
uart = UART(1, 115200)  # 使用 UART1，波特率为 115200
last_send_time = time.ticks_ms()

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8)
    data = bytearray([0xAA, 0x55, 0x05, 0, 0, 0, 0, 0x5D])
    for c in img.find_circles(threshold = 3500, x_margin = 10, y_margin = 10, r_margin = 10,
            r_min = 2, r_max = 100, r_step = 2):
        area = (c.x()-c.r(), c.y()-c.r(), 2*c.r(), 2*c.r())
        #area为识别到的圆的区域，即圆的外接矩形框
        statistics = img.get_statistics(roi=area)#像素颜色统计
        data[3] = c.x() & 0xFF  # X坐标低字节
        data[4] = c.y() & 0xFF  # Y坐标低字节
        current_time = time.ticks_ms()
        if time.ticks_diff(current_time, last_send_time) >= 500:
            uart.write(data)
            last_send_time = current_time
        print(c.x(), c.y())
        if 0<statistics.l_mode()<55 and -19<statistics.a_mode()<14 and -27<statistics.b_mode()<18:#if the circle is red
            img.draw_rectangle(area, color = (255, 255, 255))
