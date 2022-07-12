#define whd "whd"
#include "common_type.h"
#include "ModuleSDK.h"
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
#include "Gpipe.cpp"

//传输管道声明
void Gpipe(char* m);
//生成温度矩阵函数声明
//void saveTempMatrix(FLOAT_T* tempMatrix,std::string dir, short* pY16Data, const int& iWidth, const int& iHeight);
void saveTempMatrix(FLOAT_T* tempMatrix, short* pY16Data, const int& iWidth, const int& iHeight);

HANDLE_T  g_Module = 0;
int flat = 1;
//生成的温度矩阵保存的固定路径
//std::string tempDir = "/home/pi/Desktop/ArmSdk/build";



//生成实时温度矩阵，保存在文件-当前系统日期.txt中
void saveTempMatrix(FLOAT_T* tempMatrix, short* pY16Data, const int& iWidth, const int& iHeight) {
	//传输的温度字符数据
	char* m = (char*)malloc(245759 * sizeof(char));
	//最终文件的路径名称
	//char pStrPath1[256];
	if (pY16Data && g_Module) {
	//获取当前系统的时间
	//struct timeval time;
	//struct tm * mt;
	
	//gettimeofday(&time,NULL);
	//mt = localtime(&time.tv_sec);
	//if(mt != NULL){
	//	snprintf(pStrPath1, 256,"/%d%02d%02d%02d%02d%02d%04d.txt", mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday, mt->tm_hour, mt->tm_min, mt->tm_sec,time.tv_usec/1000);
	//}
	//拼接文件路径
	//std::string filename = dir + pStrPath1;
	//std::ofstream file;
	//file.open(filename);
	//printf("%s\n",pStrPath1);
	//float* tempMatrix = nullptr;
	//tempMatrix = (FLOAT_T *) malloc ((sizeof(FLOAT_T)) *iHeight * iWidth );
	//温度浮点数组
	//FLOAT_T* tempMatrix = new FLOAT_T[iHeight * iWidth];
	printf("cbFuncHandle successful!");
	if (tempMatrix) {
		//获取温度矩阵
		if (MeasureTempMatrix(g_Module, pY16Data, 0, 0, iWidth, iHeight, tempMatrix) != GUIDEIR_OK) {
			std::cout << "measure tempMatrix error  " << std::endl;
			return;
		}
		else {
			if (tempMatrix) {
				for (int i = 0, k = 0; i < iHeight * iWidth; i++) {
					char temp[30];
					snprintf(temp, 30, "%.3f", tempMatrix[i]);
					m[k] = temp[0];
					m[k + 1] = temp[1];
					m[k + 2] = temp[2];
					m[k + 3] = temp[3];
					if (i!= (iHeight * iWidth)-1) {
						m[k + 4] = ',';
					}
					k = k + 5;
				}
				Gpipe(m);
				free(m);
				//保存在txt中
				/* for (int i = 0, k = 0; i < iHeight * iWidth; i++) {
					file << std::setprecision(2) << std::fixed << tempMatrix[i] << ",";
					k++;
					if ((i + 1) % iWidth == 0) {
						file << std::endl;
						k = 0;
					}
				}*/
				//释放堆内存
				//delete []tempMatrix;
				//tempMatrix = nullptr;
			}
			else{
				DestroyModuleInstance(g_Module);
				printf("Destroy success!");
				g_Module = CreateModuleInstance(whd);
			}
		}
		//free(tempMatrix);
	}
	else{
			DestroyModuleInstance(g_Module);
			printf("Destroy success!");
			g_Module = CreateModuleInstance(whd);
	}
	
}
}

//注册回调红外数据回调函数
int cbFuncHandle(CallBackData cbData, void* param) {
	if (cbData.y16Data) {
		//获取温度矩阵文档txt
			FLOAT_T* tempMatrix = new FLOAT_T[192*256];
			//saveTempMatrix(tempMatrix,tempDir, cbData.y16Data, cbData.width, cbData.height);
			saveTempMatrix(tempMatrix, cbData.y16Data, cbData.width, cbData.height);
			delete []tempMatrix;
			tempMatrix = nullptr;
	}
	
	//每1秒测一次
	//std::this_thread::sleep_for(std::chrono::milliseconds(40));
	
	return 0;
}

int main()
{
	//int iWidth = 120;
	//int iHight = 90;
	flag:
	int isDebugging = 0;
	g_Module = CreateModuleInstance(whd);
	MeasureParamsControl(g_Module, DEBUGGING_PATH, (void*)"/home/pi/Desktop/ArmSdk/debug.log");
	MeasureParamsControl(g_Module, SET_DEBUGGING, &isDebugging);
	//RegisterImgCallBack(g_Module, cbFuncHandle, nullptr, (int)1);
	try{
		RegisterImgCallBack(g_Module, cbFuncHandle, nullptr, (int)1);	
	}
	catch(double){
		std::cerr<<"error !!!!!!!\n";
		//goto flag;
	}
	if (g_Module) {
		printf("init success!");
		//设置测温参数，距离参数，设置为0.8米
		float fDistance = 0.8f;
		if (MeasureParamsControl(g_Module, SET_DISTANCE, (void*)&fDistance) != GUIDEIR_OK) {
			std::cout << "set distance error!" << std::endl;
		}
		else {
			std::cout << "set distance is"<<fDistance << std::endl;
		}
		//设置测温参数，环境温度参数，设置为25摄氏度
		float fEnvironment = 25.0;
		if (MeasureParamsControl(g_Module, SET_AMBIENT_TEMP, (void*)&fEnvironment) != GUIDEIR_OK) {
			std::cout << "set environment error!" << std::endl;
		}
		else {
			std::cout << "set environment is"<< fEnvironment << std::endl;
		}
		//设置成像格式
		int format = 1;
		if (ImageParamsControl(g_Module, SET_FORMAT, &format)!= GUIDEIR_OK) {
			std::cout << "set format error!" << std::endl;
		}
		else {
			std::cout << "set format is" << format << std::endl;
		}
		int heratInterval = 5;
        MeasureParamsControl(g_Module, HEART_BEATING_ENABLE,(void *) &heratInterval); 
	}
	
	while (true) {
	//用户其他操作
	std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	if (g_Module) {
		DestroyModuleInstance(g_Module);
		g_Module = 0;
	}
	return 0;
}
