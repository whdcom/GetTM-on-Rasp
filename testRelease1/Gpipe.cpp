#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>

//#include <Gtest.h>
using namespace std;

//int Gtest1();
//#define BUF_SIZE 147455
#define BUF_SIZE 245759
void Gpipe(char* m);
//using std::cerr;
//using std::cout;
//using std::endl;

void Gpipe(char* m)
{   
    const char *fifo_name = "GetTempMatrix";
    int pipe_fd = -1;
    //int data_fd = -1;
    int res = 0;
    //定义打开文件的方式
    const int open_mode = O_WRONLY;
    //实际成功传送的数据长度
    //int bytes_sent = 0;
    
    //char buffer[245759 + 1];
    char *buffer;
    buffer = m;
    //判断文件是否存在，不存在则创建管道文件，FOK表示是否存在
    if (access(fifo_name, F_OK) == -1)
    {
        //创建管道
        res = mkfifo(fifo_name, 0777);
        //是否创建成功
        if (res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
            exit(EXIT_FAILURE);
            return;
        }
    }
        printf("Process %d opening FIFO O_WRONLY\n", getpid());
        //以只写方式打开管道文件
        pipe_fd = open(fifo_name, open_mode);
        //data_fd = open("data.txt", O_RDONLY);
        printf("Process %d result %d\n", getpid(), pipe_fd);
        if (pipe_fd != -1)
        {
            //int bytes_read = 0;
            int bytes_sent = 0;
            
            //bytes_read = read(data_fd, buffer, PIPE_BUF);
            
            int bytes_read = 245759;
            buffer[bytes_read] = '\0';
            if (bytes_read > 0)
            {
                
                res = write(pipe_fd, buffer, bytes_read);
                if (res == -1)
                {
                    fprintf(stderr, "Write error on pipe\n");
                    exit(EXIT_FAILURE);
                }
                else{
                    printf("send message successful!!!\n");
                }
     
                bytes_read=0;
                bytes_sent += res;
                printf("bytes_sent=%d\n",bytes_sent);
                //bytes_read = read(data_fd, buffer, PIPE_BUF);
                //buffer[bytes_read] = '\0';
                //close(data_fd);
                //this_thread::sleep_for(chrono::milliseconds(1000));
            }
        }
        else
        {
            exit(EXIT_FAILURE);
        }
        printf("Process %d finished\n", getpid());
        //exit(EXIT_SUCCESS); 
        close(pipe_fd);
        unlink(fifo_name);
  
}
