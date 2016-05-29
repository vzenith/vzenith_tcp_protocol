/*
 * vzsdk
 * Copyright 2013 - 2016, Vzenith Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include "base/logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "vzsdk/vzlprtcpsdk.h"

// 接收车牌识别结果的回调
int __STDCALL gVZLPRC_TCP_PLATE_INFO_CALLBACK(VzLPRTcpHandle handle, void *pUserData,
	const TH_PlateResult *pResult, unsigned uNumPlates,
	VZ_LPRC_RESULT_TYPE eResultType,
	unsigned char *pImgFull, int nFullSize,
	unsigned char *pImgPlateClip, int nClipSize)
{
	// 保存场景图片
	if (pImgFull != NULL && nFullSize > 0)
	{
		VzLPRTcp_ImageSaveToJpeg(pImgFull, nFullSize, "fullImg.jpg", 100);
	}

	// 保存车牌区域的图片
	if (pImgPlateClip != NULL && nClipSize > 0)
	{
		VzLPRTcp_ImageSaveToJpeg(pImgPlateClip, nClipSize, "clipImg.jpg", 100);
	}

	// 打印车牌号
	printf("plate: %s\n", pResult->license);
	return 0;
}

// 接收白名单查询结果的回调
int __STDCALL gVZLPRC_EMBEB_WLIST_RECV_CALLBACK(VzLPRTcpHandle handle, void *pUserData,
	const VZ_LPR_WLIST_VEHICLE *pWlist, unsigned uCount)
{
	if (pWlist != NULL)
	{
		// 打印白名单查询结果中的车牌号
		printf("wlist plate: %s\n", pWlist->strPlateID);
	}

	return 0;
}

// 接收485数据的回调
void __STDCALL gOnSerialRecvData(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData)
{
	if (pRecvData != NULL)
	{
		const int max_len = 1024;
		char szData[max_len] = { 0 };

		int copy_len = (uRecvSize < (max_len - 1)) ? uRecvSize : max_len - 1;
		memcpy(szData, pRecvData, copy_len);

		// 打印一体机发送过来的485数据
		printf("serial data: %s\n", szData);
	}
}

int main() {
 
	int ret = 0;
	VzLPRTcp_Setup();

	printf("start.\n");

	VzLPRTcpHandle handle = VzLPRTcp_Open("192.168.4.129", 8131, "admin", "admin");
	if (handle != NULL)
	{
		// 设置车牌识别结果的回调，用于接收实时的车牌结果。
		VzLPRTcp_SetPlateInfoCallBack(handle, gVZLPRC_TCP_PLATE_INFO_CALLBACK, NULL, TRUE);

		// 发送软件触发命令
		// 强制输出当前视频的实时结果。如果当前视频画面中无车牌，输出为“_无_”的车牌结果。
		VzLPRTcp_ForceTrigger(handle);

		ret = VzLPRTcp_SetIOOutputAuto(handle, 0, 500);

		int value = -1;
		ret = VzLPRTcp_GetGPIOValue(handle, 0, &value);
		
		// 485功能测试
		// 打开第一个485通道
		ret = VzLPRTcp_SerialStart(handle, 0, gOnSerialRecvData, NULL);
		if (ret == 0)
		{
			char szData[] = "123456";

			 //发送485数据
			 VzLPRTcp_SerialSend(handle, 0, (unsigned char *)szData, 6);
		}

		// 删除白名单
		// 空字符串表示清空所有白名单
		VzLPRTcp_DeleteWlistVehicle(handle, "");

		// 添加白名单
		VZ_LPR_WLIST_VEHICLE wlist = { 0 };
		strcpy(wlist.strPlateID, "川F12345"); // 车牌号
		wlist.bEnable = 1;										// 是否启用
		wlist.bEnableTMEnable = 1;						// 是否启用生效时间
		wlist.bEnableTMOverdule = 1;				  // 是否启用过期时间

		// 生效时间
		wlist.struTMEnable.nYear = 2015;
		wlist.struTMEnable.nMonth = 10;
		wlist.struTMEnable.nMDay = 25;
		wlist.struTMEnable.nHour = 12;
		wlist.struTMEnable.nMin = 30;
		wlist.struTMEnable.nSec = 45;

		// 过期时间
		wlist.struTMOverdule.nYear = 2015;
		wlist.struTMOverdule.nMonth = 10;
		wlist.struTMOverdule.nMDay = 30;
		wlist.struTMOverdule.nHour = 12;
		wlist.struTMOverdule.nMin = 30;
		wlist.struTMOverdule.nSec = 50;

		// 0表示为白名单，1表示为黑名单
		wlist.bAlarm = 0;

		// 车辆编码，要求必须唯一
		strcpy(wlist.strCode, "川F12345");

		// 导入白名单，如果该白名单存在则覆盖，不存在则添加。
		VzLPRTcp_ImportWlistVehicle(handle, &wlist);

		// 设置白名单查询结果的回调
		VzLPRTcp_SetWlistInfoCallBack(handle, gVZLPRC_EMBEB_WLIST_RECV_CALLBACK, NULL);

		// 查询名单
		// 车牌号为空，表示查询所有白名单
		VzLPRTcp_QueryWlistVehicle(handle, "");

		// 按回车键退出程序
		char c = 0;
		while ((c = getchar()) != 0x0a) {
		}

		// 关闭485通道
		VzLPRTcp_SerialStop(handle);

		VzLPRTcp_Close(handle);
		handle = NULL;
	}

	VzLPRTcp_Cleanup();

#ifdef WIN32
  Sleep(1000000);
#endif
  return EXIT_SUCCESS;
}