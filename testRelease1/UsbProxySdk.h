#ifndef USB_PROXY_H
#define USB_PROXY_H

#define MODULE_DATA_OK              0
#define MODULE_CONNECT_FAILED     -100
#define MODULE_PALETTE_NOT_INIT    -101
#define MODULE_IMAGECB_NOT_INIT    -102
#define MODULE_DATA_CHANNLE_READ_NOT_INIT   -103
#define MODULE_DATA_CHANNLE_WRITE_NOT_INIT  -104
#define MODULE_DATA_CHANNLE_READ_ERROE      -105

#define GUIDEIR_OK			(0)
#define GUIDEIR_ERR			(-1)

//#define GUIDE_API __declspec(dllexport)

#define GUIDE_API
#define CONSOLE_OUTPUT 

typedef struct OUT_PUT_IR_DATA {
	unsigned char* rgbData;		//旋转、翻转、放大后的图像。
	int length;
	int width;
	int height;
	unsigned short* y16Data;	//旋转、翻转后的Y16。
	int y16Length;
}IrCallBackData;

typedef int(*IRImageCallBack)(IrCallBackData irCallBackData, void *param);

typedef enum
{
	SET_DISTANCE = 1,		//距离0.5~1.2米。可以不设置，默认0.8米。参数类型：浮点型。
	GET_DISTANCE = 2,
	SET_AMBIENT_TEMP = 3,	//环境温度在用户不设置的情况下，内部自动计算环温。如果用户设置，则使用设置的值。参数类型：浮点型。
	GET_AMBIENT_TEMP = 4,	//获取环温得到的是一个近似值，用户要根据实际情况减去偏移量才能得到真实环温。计算体内温度会用到环温。
	SET_KF = 5,				//校准值可以不设置。
	GET_KF = 6,				//参数类型：KFB_INFO
	CALC_CENTRAL_ENABLE = 7,	//开启计算屏幕中心温。无参数。用户在需要时开启此功能。
	CALC_CENTRAL_DISABLE = 8,	//关闭计算屏幕中心温。无参数。
	GET_CENTRAL_TEMP = 9,		//获取屏幕中心温。参数类型：浮点型。
	RESET_NOW = 10,				//发送重启命令，设备立刻重启。
	HEART_BEATING_ENABLE = 11,	//开启心跳功能（openstream成功有图像数据回调后才能开启），心跳间隔时间5~15秒，如果参数设置为0则默认5秒，参数类型：整形。
	HEART_BEATING_DISABLE = 12,	//关闭心跳功能。
	DEBUGGING_ENABLE = 13,		//开启调试，信息输出到日志文件debug_out.log。日志文件默认保存在程序同级目录下。
	DEBUGGING_DISABLE = 14,		//关闭调试。
	DEBUGGING_PATH = 15,		//设置日志保存的目录，要在GuideSDKInit之前设置。必须是已存在的目录，例如/home/app/log。注意不能以/结尾，长度不要超过128。参数类型：字符串，以 \0 结束。
	GET_JPM_TEMP = 16,			//获取焦平面温度。参数类型：浮点型。
	SET_EMISS,
	GET_EMISS
}CMD_PARAMETER_TYPE;

typedef enum
{
	SET_BRIGHTNESS = 1,			//亮度0-256。默认60. 参数类型：整形。在openstream之后调用。
	GET_BRIGHTNESS = 2,
	SET_CONTRAST = 3,			//对比度0-256。默认256. 参数类型：整形。在openstream之后调用。
	GET_CONTRAST = 4,
	SET_PALETTE_INDEX = 5,		//设置色带号，范围0-10. 默认0.在openstream之后调用。
	GET_PALETTE_INDEX = 6,
	SET_SCALE_TIMES = 7,		//设置图像缩放倍数。默认1.0，不缩放。必须在openstream之前init之后调用，否则无效。参数类型：浮点型。
	GET_SCALE_TIMES = 8,		//设置缩放后仅RGB图像放大。
	SET_ROTATION = 9,			//设置旋转方式，默认0。0，不旋转。1，旋转90度。2，旋转180度。3，旋转270度。
	GET_ROTATION = 10,			//设置旋转后RGB和Y16数据均旋转。
	SET_FLIP = 11,				//设置是否左右翻转，默认0。0，不翻转。1，左右翻转。
	GET_FLIP = 12,				//设置翻转后RGB和Y16数据均翻转。
	RGB_CALLBACK_ENABLE = 13,	//启用成像功能并回调RGB数据。在openstream之后调用。
	RGB_CALLBACK_DISABLE = 14,	//关闭转RGB图像功能，降低CPU负荷。此时回调数据中rgb length为0。根据自身需求开关。在openstream之后调用。
	GRAB_FRAME_ONCE = 15,		//抓取一次成像过程中各阶段帧数据，srcXArr1.raw等文件保存在可执行程序同级目录下。
	SET_FRAME_RATE = 16,
	GET_FRAME_RATE = 17,
}CMD_IMAGING_TYPE;

typedef enum
{
	SHUTTER_NOW = 1,			//打快门
	AUTO_SHUTTER_ENABLE = 2,	//启用自动快门，默认启用。
	AUTO_SHUTTER_DISABLE = 3,	//关闭自动快门
	SET_SHUTTER_TIME = 4,		//设置自动快门时间，单位秒。默认30秒。
	GET_SHUTTER_TIME = 5
}CMD_SHUTTER_TYPE;

typedef struct
{
	char serialNum[64];
	char firmwareVersion[32];
	char sdkVersion[32];
}FACTORY_VERSION;

/**
* @brief:  SDK初始化。
* @access: public 
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKInit();

/**
* @brief:  SDK退出。
* @access: public 
* @return: 无。
* @note:  
**/
extern "C" GUIDE_API void GuideSDKExit();

/**
* @brief:  打开流。
* @access: public 
* @param:  IRImageCallBack irImageCallBack
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKOpenStream(IRImageCallBack irImageCallBack, void *param);

/**
* @brief:  关闭流。
* @access: public 
* @return: 无
* @note:  
**/
extern "C" GUIDE_API void GuideSDKCloseStream();

/**
* @brief:  成像控制。
* @access: public 
* @param:  CMD_IMAGING_TYPE type
* @param:  void * param 传地址。
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKImagingControl(CMD_IMAGING_TYPE type, void *param);

/**
* @brief:  快门控制。
* @access: public 
* @param:  CMD_SHUTTER_TYPE type
* @param:  void * param 传地址。
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKShutterControl(CMD_SHUTTER_TYPE type, void *param);

/**
* @brief:  参数控制。
* @access: public 
* @param:  CMD_PARAMETER_TYPE type
* @param:  void * param
* @return: extern " GUIDE_API int
* @note:  
**/
extern "C" GUIDE_API int GuideSDKParameterControl(CMD_PARAMETER_TYPE type, void *param);

/**
* @brief:  根据Y16值计算体表温度。
* @access: public 
* @param:  short y16值。
* @return: float 返回体表温度。
* @note:  
**/
extern "C" GUIDE_API float GuideSDKCalcTempByY16(short y16);

/**
* @brief:  人体测温时使用。根据体表温度计算体内温度。注意使用SET_AMBIENT_TEMP设置正确的环温。
* @access: public 
* @param:  float temp 体表温度。
* @return: float 返回体内温度。
* @note:   如果使用GET_AMBIENT_TEMP，获取环温得到的是一个近似值，用户要根据实际情况减去偏移量才能得到真实环温。此种情况下用GuideSDKCalcBodyTempEx得到体内温度。
**/
extern "C" GUIDE_API float GuideSDKCalcBodyTemp(float temp);

/**
* @brief:  人体测温时使用。根据体表温度计算体内温度，传入环温。注意设置正确的环温。
* @access: public
* @param:  float temp 体表温度。
* @param:  float envirTemp 环境温度。
* @return: float 返回体内温度。
* @note:   如果使用GET_AMBIENT_TEMP，获取环温得到的是一个近似值，用户要根据实际情况减去偏移量才能得到真实环温。
**/
extern "C" GUIDE_API float GuideSDKCalcBodyTempEx(float temp, float envirTemp);

/**
* @brief:  由用户选择使用新或旧算法计算体内温度。根据体表温度计算体内温度，传入环温。注意设置正确的环温。
* @access: public
* @param:  float temp 体表温度。
* @param:  float envirTemp 环境温度。
* @param:  int type 由用户选择：1，采用旧算法；2，采用新算法。
* @return: float 返回体内温度。
* @note:   如果使用GET_AMBIENT_TEMP，获取环温得到的是一个近似值，用户要根据实际情况减去偏移量才能得到真实环温。
**/
extern "C" GUIDE_API float GuideSDKCalcBodyTempAnother(float temp, float envirTemp, int type);

/**
* @brief:  工业测温时使用。设置测温范围，1表示工业测温常温段（默认值），2表示工业测温高温段。
* @access: public
* @param:  int range 测温范围。1表示工业测温常温段（默认值），2表示工业测温高温段。
* @return: 成功返回GUIDEIR_OK。如果失败返回GUIDEIR_ERR。
* @note:   
**/
extern "C" GUIDE_API int GuideSDKSetRange(int range);

/**
* @brief:  查询出厂信息，SN序列号 SDK版本号等。
* @access: public 
* @param:  FACTORY_VERSION * info
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKFactoryVersion(FACTORY_VERSION *info);

/**
* @brief:  固件升级。如果返回GUIDEIR_OK，可调用RESET_NOW让模组进入升级流程。模组升级过程持续5-10秒，切勿断电。
*          如果返回GUIDEIR_ERR，在确保升级文件无误的情况下可再尝试一次GuideSDKFirmwareUpdate。
* @access: public 
* @param:  unsigned char * updateData 升级数据
* @param:  int length 长度
* @return: 如果失败返回GUIDEIR_ERR。
* @note:  
**/
extern "C" GUIDE_API int GuideSDKFirmwareUpdate(unsigned char *updateData, int length);

//以下是自动校温接口
typedef struct
{
	short B;
	short KF;
	short farKf;
	short farB;
	short B2;
	short KF2;
	short farKf2;
	short farB2;
}KFB_INFO;

typedef void(*CollectFinishCallBack)(void *param);
/**
* @brief:  开始采集数据，完成后用回调函数通知。
* @access: public
* @param:  float distance
* @param:  float temper
* @param:  CollectFinishCallBack cb
* @param:  void *param
* @return: 如果失败返回GUIDEIR_ERR。如果成功返回GUIDEIR_OK。
* @note:
**/
extern "C" GUIDE_API int GuideSDKStartCollect(float distance, float temper, CollectFinishCallBack cb, void *param);
/**
* @brief:  开始自动校温。
* @access: public
* @param:  float *Kf
* @param:  float *B
* @return: 如果失败返回GUIDEIR_ERR。如果成功返回GUIDEIR_OK。
* @note:
**/
extern "C" GUIDE_API int GuideSDKStartCalibrate(float *Kf, float *B); 
/**
* @brief:  二次自动校温。
* @access: public
* @param:  float *Kf
* @param:  float *B
* @return: 如果失败返回GUIDEIR_ERR。如果成功返回GUIDEIR_OK。
* @note:
**/
extern "C" GUIDE_API int GuideSDKStartCalibrateEx(float *Kf, float *B);

//以下是操作模组接口
typedef enum
{
	SET_MODULE_SN,	//写入模组SN，参数param是SN字符串，以'\0'结束。不能超过64B。
	GET_MODULE_SN,	//获取模组SN，参数param是长度为64B的buffer。
	GET_FPA_TEMP,	//获取焦温，参数param是float类型。
	GET_SHUTTER_TEMP,//获取快门温，参数param是float类型。
	GET_LENS_TEMP,	//获取镜筒温，参数param是float类型。
}CMD_MODULE_TYPE;
/**
* @brief:  操作模组。
* @access: public
* @param:  CMD_MODULE_TYPE type
* @param:  void *param
* @return: 如果失败返回GUIDEIR_ERR。如果成功返回GUIDEIR_OK。
* @note:
**/
extern "C" GUIDE_API int GuideSDKModuleControl(CMD_MODULE_TYPE type, void *param);

//以下是扩展接口
typedef struct BG_DATA {
	unsigned short* bgData;
	int dataLen;
	int w;
	int h;
}BgData;
typedef void(*BdCallBack)(BgData bgData, void *param);
/**
* @brief:  注册本底数据回调函数。
* @access: public 
* @param:  BdCallBack cb
* @param:  void * param
* @return: 无
* @note:  
**/
extern "C" GUIDE_API void GuideSDKRegisterBD(BdCallBack cb, void *param);

#endif
