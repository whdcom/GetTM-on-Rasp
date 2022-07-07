#!/usr/bin/python3.9
import os
import errno
import struct
import threading

def recv_pipe(PIPE_NAME, PIPE_BUFFER_SIZE):
    while True:
        try:
            os.mkfifo(PIPE_NAME)
        except OSError as oe: 
            if oe.errno != errno.EEXIST:
                print("Error occured")
            with open(PIPE_NAME,'rb') as fifo:
                print("FIFO opened")
                
                data = fifo.read(PIPE_BUFFER_SIZE)
                print(len(data))
                print(type(data))
                #for e in data:
                #    print(e+ "a")

                if len(data) == 0:
                    continue
                    print("Writer error2")

#print(data)
#将C++传来的二进制数据进行格式化为字符串数据相当于元组字符串,<表示小端内存存储，而>表示大端内存存储
                recv_msg = struct.unpack('<245759s', data)
#对二进制字符串进行解码
                recv_msg = recv_msg[0].decode("utf-8")
                print(recv_msg)
#def run():
#  os.system('./GTest3')
if __name__ == "__main__":
    pipe_name = 'GetTempMatrix'
    pipe_buffer_size = 245759
    receive_thread = threading.Thread(target=recv_pipe, args=(pipe_name, pipe_buffer_size))
#    write_thread = threading.Thread(target=run)
#    write_thread.start()
    while True:
        try:
            receive_thread.start()
        except BaseException as e:
            #print("Exception1",e)
            continue
