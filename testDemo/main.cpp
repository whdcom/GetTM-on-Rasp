#include "ModuleSDK.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

HANDLE_T  g_Module = 0;

//处理红外原始数据，通过原始数据针对任意点，通过y16数据进行测温，
//同时也可以通过原始y16得到最高和最低的y16值，算出最高温和最低温，避免全帧测温计算最高最低温
//此处理可以单独放在独立线程中执行，防止在cbFuncHandle中执行，影响回调函数返回
void handleY16Data(short* pY16Data, const int& iLen, const int& iWidth, const int& iHeight) {
    if (iLen != iWidth * iHeight) {
        std::cout << "invalid data size ,drop it!" << std::endl;
        return;
    }
    //模拟点测温
    int iPosX = 100;
    int iPosY = 60;

    if (pY16Data && g_Module) {
        int iPosMeasure = iWidth * iPosY + iPosX;
        if (iPosMeasure < iWidth * iHeight) {
            short iPosY16 = pY16Data[iPosMeasure];
            float fSurfaceT = 0;
            if (MeasureTempByY16(g_Module, iPosY16, &fSurfaceT) != GUIDEIR_OK) {
                std::cout << "measure point error , posx : " << iPosX << " posy: " << iPosY << std::endl;
                return;
            }
            else {
                //针对人体测温，需要将体表温度映射到人体温度
                float env;
                MeasureParamsControl(g_Module, GET_AMBIENT_TEMP, &env);
                float fBodyT = 0;
                if (MeasureBodyTemp(g_Module, fSurfaceT, env, &fBodyT) == GUIDEIR_OK) {
                    printf("surface temperature:%f body temperature:%f env:%f \n", fSurfaceT, fBodyT, env);
                }
            }
        }
    }
}

//处理红外实时视频数据，数据格式为yuv422，用于调用端显示使用，
//同时也可以根据实际情况，在实时画面中绘制自定义信息
//此处理可以单独放在独立线程中执行，避免在在cbFuncHandle中执行，影响回调函数返回
void handleYuvData(unsigned char* pYuvData, const int& iLen, const int& iWidth, const int& iHeight) {
    //用户可以使用yuv422进行实时画面绘制操作
}

//注册回调红外数据回调函数
int cbFuncHandle(CallBackData cbData, void * param) {
    //实时画面更新操作
    if (cbData.yuvData) {
        handleYuvData(cbData.yuvData, cbData.yuvLength, cbData.width, cbData.height);
        //printf("yuvLength=%d\n", cbData.yuvLength);
    }
    //实时y16测温数据更新操作
    if (cbData.y16Data) {
        handleY16Data(cbData.y16Data, cbData.y16Length, cbData.width, cbData.height);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int iWidth = 120;
    int iHight = 90;
    int isDebugging = 0;
    /******************************初始化sdk模块******************************/
    g_Module = CreateModuleInstance("");
    //MeasureParamsControl(g_Module, DEBUGGING_PATH, (void *)"/home/app/log");
    //MeasureParamsControl(g_Module, SET_DEBUGGING, &isDebugging);
    RegisterImgCallBack(g_Module, cbFuncHandle, nullptr, 1);
    if (g_Module) {
        /******************************设置测温参数******************************/
        //设置测温参数，距离参数，设置为0.8米
        float fDistance = 0.8;
        if (MeasureParamsControl(g_Module, SET_DISTANCE, (void *)&fDistance) != GUIDEIR_OK) {
            std::cout << "set distance error!" << std::endl;
        }
        //设置测温参数，环境温度参数，设置为25摄氏度
        float fEnvironment = 25.0;
        if (MeasureParamsControl(g_Module, SET_AMBIENT_TEMP, (void *)&fEnvironment) != GUIDEIR_OK) {
            std::cout << "set environment error!" << std::endl;
        }

        int format = 2;
        ImageParamsControl(g_Module, SET_FORMAT, &format);
    }

    /******************************用户线程其他操作******************************/
    while (true) {
        //用户其他操作
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    /******************************释放sdk模块******************************/
    //程序退出时调用
    if (g_Module) {
        DestroyModuleInstance(g_Module);
        g_Module = 0;
    }
    return 0;
}
