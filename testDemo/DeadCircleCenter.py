import math
import sys
import traceback

import win32file
import win32pipe
import struct
import threading
import cv2
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import os
import time

import test_live3

# from ishow import ishow
path=r"D:\PythonProjects\Release\GuTest.exe"


def recv_pipe(PIPE_NAME, PIPE_BUFFER_SIZE,detect_total_num=20,active_radius=80,res_delay_show_time=15):
    # detecting_time = 1  # 检测时间
    # detect_total_num=detecting_time*25#一秒25帧，时间*每秒的帧数，得到要测试的总的帧数。
    #detect_total_num=20#设置要测试的图片的数量
    index = 1
    first_center=None
    #active_radius=80
    while True:
        named_pipe = win32pipe.CreateNamedPipe(PIPE_NAME,
                                               win32pipe.PIPE_ACCESS_DUPLEX,
                                               win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT | win32pipe.PIPE_READMODE_MESSAGE,
                                               win32pipe.PIPE_UNLIMITED_INSTANCES,
                                               PIPE_BUFFER_SIZE,
                                               PIPE_BUFFER_SIZE, 500, None)
        try:

            while True:
                 try:
                    win32pipe.ConnectNamedPipe(named_pipe, None)
                    # os.system(path)
                    data = win32file.ReadFile(named_pipe, PIPE_BUFFER_SIZE, None)
                    if data is None:
                        continue
                    # print("Received msg:", data)
                    #将C++传来的二进制数据进行格式化为字符串数据相当于元组字符串,<表示小端内存存储，而>表示大端内存存储
                    recv_msg = struct.unpack('<245759s', data[1])
                    #对二进制字符串进行解码
                    recv_msg = recv_msg[0].decode("utf-8")
                    frame=test_live3.get_raw_img(recv_msg)#温度图
                    #binary_frame=test_live3.get_binary_img(recv_msg)
                    center=test_live3.detect_one_frame(recv_msg)
                    if center is None:
                        continue
                    if index==1:
                        first_center=center
                    else:
                        cx,cy=center
                        if (math.sqrt(((cx-first_center[0])**2+(cy-first_center[1])**2)))> active_radius:
                            continue
                        # else:
                        #     cv2.circle(frame,first_center,active_radius,(212,100,77),1)
                    test_live3.judgingDeadOrLive(center)
                    print(index,"-----------------------------------------------------------------------------------------")
                    if index ==detect_total_num and center is not None:
                        print("i need to be excuted")
                        result_frame=None
                        try:
                            result_frame,is_live=test_live3.getJudgeResult(detect_total_num,center,frame)
                        except Exception as e:
                            traceback.print_exc()
                        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                        cv2.namedWindow("result_frame",0)
                        cv2.imshow("result_frame",result_frame)
                        cv2.imwrite("result_frame.jpg",result_frame)
                        cv2.waitKey(res_delay_show_time)
                        index=0

                    timestr = time.strftime("%Y%m%d-%H%M%S")
                    print(timestr)
                    index+=1
                 except BaseException as e:
                     traceback.print_exc()
                     break
        finally:
            try:
                win32pipe.DisconnectNamedPipe(named_pipe)
            except BaseException as e:
                print("Exception2:", e)
                break
# //启动程序
def run():
    os.system(path)

if __name__ == "__main__":
    pipe_name = r"\\.\pipe\test_pipe"
    pipe_buffer_size = 245759
    test_live3.setDeadRadius(12)  # 设置死亡半径。默认为12，半径越大，越可能会死。
    test_live3.setTemptureThresh(30)#设置温度阈值，默认为30
    total_detect_num=40#检测的图片的数量，默认是20
    active_radius=80#活动半径，活动半径之外视为噪音，为了去除噪声用
    res_delay_show_time=15#cv2.waitKey时间，即结果要停留展示多久，默认是15ms,必须要大于0
    receive_thread = threading.Thread(target=recv_pipe, args=(pipe_name,
                                                              pipe_buffer_size,
                                                              total_detect_num,
                                                              active_radius,
                                                              res_delay_show_time
                                                              ))
    write_thread = threading.Thread(target=run)

    write_thread.start()
    receive_thread.start()