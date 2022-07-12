import sys
sys.path=['/home/pi/Desktop/raspberryGetTemp/testDemo', '/usr/lib/python39.zip', '/usr/lib/python3.9', '/usr/lib/python3.9/lib-dynload', '/home/pi/.local/lib/python3.9/site-packages', '/usr/local/lib/python3.9/dist-packages', '/usr/lib/python3/dist-packages', '/usr/lib/python3.9/dist-packages']
import os
import errno
import struct
import threading

import math
import sys
import traceback

import sPubilsh
import cv2
import numpy as np
import os
import time

import test_live3


def recv_pipe(PIPE_NAME, PIPE_BUFFER_SIZE,detect_total_num=20,active_radius=80,res_delay_show_time=15):
    index = 1
    first_center=None
    while True:
        try:
            os.mkfifo(PIPE_NAME)
        except OSError as oe: 
            if oe.errno != errno.EEXIST:
#               print("Error occured")
                pass
            with open(PIPE_NAME,'rb') as fifo:
#               print("FIFO opened")
                data = fifo.read(PIPE_BUFFER_SIZE)
                print(len(data))
                print(type(data))
                #for e in data:
                #    print(e+ "a")

                if len(data) == 0:
                    continue
#                    print("Writer error2")

#print(data)
#将C++传来的二进制数据进行格式化为字符串数据相当于元组字符串,<表示小端内存存储，而>表示大端内存存储
                recv_msg = struct.unpack('<245759s', data)
#对二进制字符串进行解码
                recv_msg = recv_msg[0].decode("utf-8")
#                print(recv_msg)
                frame=test_live3.get_raw_img(recv_msg)#温度图
                #binary_frame=test_live3.get_binary_img(recv_msg)
                center=test_live3.detect_one_frame(recv_msg)
                if center is None:
                    continue
                if center[0]==127 or center[0]==128 or center[0]==129:
                    continue
                if index==1:
                    if center[0]>40 and center[0]<152 and center[1]>50 and center[1]<186:
                        first_center=center
                    else:
                        continue
                else:
                    cx,cy=center
                    if (math.sqrt(((cx-first_center[0])**2+(cy-first_center[1])**2)))> active_radius:
                        continue
                    #else:
                    #    cv2.circle(frame,first_center,active_radius,(212,100,77),1)
                test_live3.judgingDeadOrLive(center)
                print(index,"-----------------------------------------------------------------------------------------")
                if index ==detect_total_num and center is not None:
#                    print("i need to be excuted")
                    result_frame=None
                    try:
                        result_frame,dead=test_live3.getJudgeResult(detect_total_num,center,frame)
                        img =result_frame
                        breed = "鸡"
                        state = "正常"
                        device_id = "nanmu_chicken_001"
                        if dead:
                             state="异常"
                        sPubilsh.send_message(device_id,breed,state,img)
                    except Exception as e:
                        traceback.print_exc()
#                   print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    #cv2.namedWindow("result_frame",0)
                    #cv2.imshow("result_frame",result_frame)
                    #cv2.imwrite("result_frame.jpg",result_frame)
                    #cv2.waitKey(res_delay_show_time)
                    index=0

                timestr = time.strftime("%Y%m%d-%H%M%S")
                print(timestr)
                index+=1
#def run():
#  os.system('./GTest3')
if __name__ == "__main__":
    pipe_name = 'GetTempMatrix'
    pipe_buffer_size = 245759
#    receive_thread = threading.Thread(target=recv_pipe, args=(pipe_name, pipe_buffer_size))
#    write_thread = threading.Thread(target=run)
#    write_thread.start()
    test_live3.setDeadRadius(12)  # 设置死亡半径。默认为12，半径越大，越可能会死。
    test_live3.setTemptureThresh(30)#设置温度阈值，默认为30
    total_detect_num=7#检测的图片的数量，默认是20
    active_radius=120#活动半径，活动半径之外视为噪音，为了去除噪声用
    res_delay_show_time=15#cv2.waitKey时间，即结果要停留展示多久，默认是15ms,必须要大于0
    receive_thread = threading.Thread(target=recv_pipe, args=(pipe_name,
                                                              pipe_buffer_size,
                                                              total_detect_num,
                                                              active_radius,
                                                              res_delay_show_time
                                                              ))
    while True:
        try:
            receive_thread.start()
        except BaseException as e:
            #print("Exception1",e)
            continue
#    write_thread = threading.Thread(target=run)
#    write_thread.start()