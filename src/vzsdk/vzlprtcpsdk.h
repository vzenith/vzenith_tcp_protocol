#ifndef _VZ_LPRC_TCP_SDK_H_
#define _VZ_LPRC_TCP_SDK_H_
#include "vzlprtcpsdkdefine.h"

#ifdef WIN32
	#ifdef _DEV_API_
		#define VZ_LPRC_TCP_API __declspec(dllexport)
	#else
		#define VZ_LPRC_TCP_API __declspec(dllimport)
	#endif

	#define __STDCALL __stdcall
#else
	#define	VZ_LPRC_TCP_API
	#define __STDCALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @defgroup group_global 全局操作函数
* @defgroup group_device 单个设备操作函数
* @defgroup group_callback 回调函数
*/

/**
*  @brief 全局初始化
*  @return 0表示成功，-1表示失败
*  @note 在所有接口调用之前调用
*  @ingroup group_global
*/
extern "C" int __STDCALL VzLPRTcp_Setup();

/**
*  @brief 全局释放
*  @note 在程序结束时调用，释放SDK的资源
*  @ingroup group_global
*/
VZ_LPRC_TCP_API void __STDCALL VzLPRTcp_Cleanup();

/**
*  @brief 打开一个设备
*  @param  [IN] pStrIP 设备的IP地址
*  @param  [IN] wPort 设备的端口号
*  @param  [IN] pStrUserName 访问设备所需用户名
*  @param  [IN] pStrPassword 访问设备所需密码
*  @return 返回设备的操作句柄，当打开失败时，返回0
*  @ingroup group_device
*/
VZ_LPRC_TCP_API VzLPRTcpHandle __STDCALL VzLPRTcp_Open(const char *pStrIP, WORD wPort, const char *pStrUserName, const char *pStrPassword);

/**
*  @brief 关闭一个设备
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_Close(VzLPRTcpHandle handle);

/**
*  @brief 通过IP地址关闭一个设备
*  @param  [IN] pStrIP 设备的IP地址
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_CloseByIP(const char *pStrIP);

/**
*  @brief 获取设备序列号
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [OUT] pSN 设备序列号
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetDeviceSN(VzLPRTcpHandle handle, char* pSN);

/**
*  @brief 获取连接状态
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN/OUT] pStatus 输入获取状态的变量地址，输出内容为 1已连上，0未连上
*  @return 0表示成功，-1表示失败
*  @note   用户可以周期调用该函数来主动查询设备是否断线，以及断线后是否恢复连接
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_IsConnected(VzLPRTcpHandle handle, BYTE *pStatus);

/**
*  @brief 通过该回调函数获得车牌识别信息
*  @param  [OUT] handle			由VzLPRTcp_Open函数获得的句柄
*  @param  [OUT] pUserData		回调函数的上下文
*  @param  [OUT] pResult		车牌信息数组首地址，详见结构体定义 TH_PlateResult
*  @param  [OUT] uNumPlates		车牌数组中的车牌个数
*  @param  [OUT] eResultType	车牌识别结果类型，详见枚举类型定义VZ_LPRC_RESULT_TYPE
*  @param  [OUT] pImgFull		当前帧的图像内容首地址
*  @param  [OUT] nFullSize		图像在内存中的大小
*  @param  [OUT] pImgPlateClip	当前帧中车牌区域的图像内容数组
*  @param  [OUT] nClipSize		车牌区域图像在内存中的大小
*  @return 0表示成功，-1表示失败
*  @note   如果需要该回调函数返回截图内容 pImgFull 和pImgPlateClip，需要在设置回调函数（VzLPRTcp_SetPlateInfoCallBack）时允许截图内容，否则该回调函数返回的这两个指针为空；
*  @ingroup group_callback
*/
typedef int (__STDCALL *VZLPRC_TCP_PLATE_INFO_CALLBACK)(VzLPRTcpHandle handle, void *pUserData,
													const TH_PlateResult *pResult, unsigned uNumPlates,
													VZ_LPRC_RESULT_TYPE eResultType,
													unsigned char *pImgFull, int nFullSize, 
													unsigned char *pImgPlateClip, int nClipSize);

/**
*  @brief 设置识别结果的回调函数
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] func 识别结果回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @param  [IN] bEnableImage 指定识别结果的回调是否需要包含截图信息：1为需要，0为不需要
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetPlateInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_PLATE_INFO_CALLBACK func, void *pUserData, int bEnableImage);


/**
*  @brief 将图像保存为JPEG到指定路径
*  @param  [IN] pImgBuf  图像内存首地址
*  @param  [IN] nImgSize 图像内存大小
*  @param  [IN] pSavePathName 设带绝对路径和JPG后缀名的文件名字符串
*  @param  [IN] nQuality（预留） JPEG压缩的质量，取值范围1~100；
*  @return 0表示成功，-1表示失败
*  @note   给定的文件名中的路径需要存在
*  @ingroup group_global
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ImageSaveToJpeg(unsigned char *pImgBuf, int nImgSize, const char *pSavePathName, int nQuality);

/**
*  @brief 通过该回调函数获得透明通道接收的数据
*  @param  [IN] nSerialHandle VzLPRTcp_SerialStart返回的句柄
*  @param  [IN] pRecvData	接收的数据的首地址
*  @param  [IN] uRecvSize	接收的数据的尺寸
*  @param  [IN] pUserData	回调函数上下文
*  @ingroup group_callback
*/
typedef void (__STDCALL *VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK)(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData);

/**
*  @brief 开启透明通道
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @param  [IN] func 接收数据的回调函数
*  @param  [IN] pUserData 接收数据回调函数的上下文
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialStart(VzLPRTcpHandle handle, int nSerialPort,
												  VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func, void *pUserData);
/**
*  @brief 透明通道发送数据
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @param [IN] pData 将要传输的数据块的首地址
*  @param [IN] uSizeData 将要传输的数据块的字节数
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialSend(VzLPRTcpHandle handle, int nSerialPort, const unsigned char *pData, unsigned uSizeData);

/**
*  @brief 透明通道停止发送数据
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] nSerialPort 指定使用设备的串口序号：0表示第一个串口，1表示第二个串口
*  @return 0表示成功，其他值表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialStop(VzLPRTcpHandle handle, int nSerialPort);

/**
*  @brief 设置IO输出，并自动复位
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] uChnId IO输出的通道号，从0开始
*  @param  [IN] nDuration 延时时间，取值范围[500, 5000]毫秒
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetIOOutputAuto(VzLPRTcpHandle handle, unsigned uChnId, int nDuration);

/**
*  @brief 通过该回调函数获得设备的一般状态信息
*  @param  [IN] handle		由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] pUserData	回调函数上下文
*  @param  [IN] eNotify		通用信息反馈类型
*  @param  [IN] pStrDetail	详细描述字符串
*  @ingroup group_callback
*/
typedef void (__STDCALL *VZLPRC_TCP_COMMON_NOTIFY_CALLBACK)(VzLPRTcpHandle handle, void *pUserData,
														VZ_LPRC_COMMON_NOTIFY eNotify, const char *pStrDetail);

/**
*  @brief 设置设备连接反馈结果相关的回调函数
*  @param  [IN] func 设备连接结果和状态，通过该回调函数返回
*  @param  [IN] pUserData 回调函数中的上下文
*  @return 0表示成功，-1表示失败
*  @ingroup group_global
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *pUserData);


/**
*  @brief 根据ID获取车牌图片
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] id     车牌记录的ID
*  @param  [IN] pdata  存储图片的内存
*  @param  [IN][OUT] size 为传入传出值，传入为图片内存的大小，返回的是获取到jpg图片内存的大小
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_LoadImageById(VzLPRTcpHandle handle, int id, void *pdata, int* size);

/**
*  @brief 获取GPIO的状态
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] gpioIn 数据为0或1
*  @param  [OUT] value 0代表短路，1代表开路
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetGPIOValue(VzLPRTcpHandle handle, int gpioIn, int *value);

/**
*  @brief 开启脱机检查，实现脱机自动切换白名单的功能。
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] pUserData 接收数据回调函数的上下文
*  @note   有效前提：设备的“输入输出”页面的“白名单验证”TAB页中的“白名单启用条件”设置为“脱机自动启用”
*  @note   调用该接口，设备与调用该接口的客户端之间就会形成绑定，如果客户端与设备之间连接正常，那么设备上的白名单就为停用状态，不会输出IO信号控制道闸抬杆和输出485推送协议；
*  @note   如果客户端与设备设备之间的网络断了，或者客户端关闭，或者调用了关闭脱机检查（CancelOfflineCheck）接口，那么设备上的白名单将启用，设备依据白名单控制道闸抬杆和输出485推送协议；
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetOfflineCheck(VzLPRTcpHandle handle);

/**
*  @brief 关闭脱机检查
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] pUserData 接收数据回调函数的上下文
*  @note   有效前提：设备的“输入输出”页面的“白名单验证”TAB页中的“白名单启用条件”设置为“脱机自动启用”
*  @note   关闭脱机检查，设备基于白名单来输出IO信号控制道闸抬杆和输出485推送协议。
*  @return 返回值为0表示成功，返回-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_CancelOfflineCheck(VzLPRTcpHandle handle);

/**
*  @brief 发送软件触发信号，强制处理当前时刻的数据并输出结果
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @return 0表示成功，-1表示失败
*  @note   车牌识别结果通过回调函数的方式返回，如果当前视频画面中无车牌，则回调函数不会被调用
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ForceTrigger(VzLPRTcpHandle handle);

/**
* @brief 获取 LastError
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetLastError( );


/**
*  @brief 根据ID获取车牌记录
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] id     车牌记录的ID
*  @param  [IN] pResult车牌记录
*  @return 0表示成功，-1表示失败
*  @note   车牌识别结果通过回调函数的方式返回，如果当前视频画面中无车牌，则回调函数不会被调用
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_LoadRecordById(VzLPRTcpHandle handle, int id, TH_PlateResult *pResult);

/**
*  @brief 获取设备的IP
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @return 返回设备的IP
*  @ingroup group_device
*/
VZ_LPRC_TCP_API const char* __STDCALL VzLPRTcp_GetIP(VzLPRTcpHandle handle);

/**
*  @brief 设置脱机结果的回调函数
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] func 识别结果回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @param  [IN] bEnableImage 指定识别结果的回调是否需要包含截图信息：1为需要，0为不需要
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetOfflinePlateInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_PLATE_INFO_CALLBACK func, void *pUserData, int bEnableImage);

/**
*  @brief 导入一条白名单记录
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] pLprWlistVehicle 白名单记录
*  @return 0表示成功，-1表示失败
*  @note   如果白名单记录已经存在，则更新。不存在，则添加到白名单数据库中。
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ImportWlistVehicle(VzLPRTcpHandle handle, VZ_LPR_WLIST_VEHICLE *pWlist);

/**
*  @brief 根据车牌号删除白名单
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] plateCode 车牌号
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_DeleteWlistVehicle(VzLPRTcpHandle handle, const char* plateCode);

/**
*  @brief 根据车牌号查询白名单
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] plateCode 车牌号
*  @return 0表示成功，-1表示失败
*  @note   白名单通过回调输出。车牌号为空，返回所有的白名单。
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_QueryWlistVehicle(VzLPRTcpHandle handle, const char* plateCode);

/**
*  @brief 通过该回调函数获得白名单查询结果
*  @param  [IN] handle			由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] pUserData		回调函数的上下文
*  @param  [IN] pWlist			白名单数组首地址，详见结构体定义 VZ_LPR_WLIST_VEHICLE
*  @param  [IN] uCount			白名单数组中的车牌个数
*  @return 0表示成功，-1表示失败
*  @ingroup group_callback
*/
typedef int (__STDCALL *VZLPRC_TCP_WLIST_RECV_CALLBACK)(VzLPRTcpHandle handle, void *pUserData,
														  const VZ_LPR_WLIST_VEHICLE *pWlist, unsigned uCount);

/**
*  @brief 设置白名单结果的回调函数
*  @param  [IN] handle 由VzLPRTcp_Open函数获得的句柄
*  @param  [IN] func 白名单回调函数，如果为NULL，则表示关闭该回调函数的功能
*  @param  [IN] pUserData 回调函数中的上下文
*  @return 0表示成功，-1表示失败
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetWlistInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_WLIST_RECV_CALLBACK func, void *pUserData);

#ifdef __cplusplus
}
#endif

#endif