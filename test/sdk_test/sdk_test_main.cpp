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

// ���ճ���ʶ�����Ļص�
int __STDCALL gVZLPRC_TCP_PLATE_INFO_CALLBACK(VzLPRTcpHandle handle, void *pUserData,
	const TH_PlateResult *pResult, unsigned uNumPlates,
	VZ_LPRC_RESULT_TYPE eResultType,
	unsigned char *pImgFull, int nFullSize,
	unsigned char *pImgPlateClip, int nClipSize)
{
	// ���泡��ͼƬ
	if (pImgFull != NULL && nFullSize > 0)
	{
		VzLPRTcp_ImageSaveToJpeg(pImgFull, nFullSize, "fullImg.jpg", 100);
	}

	// ���泵�������ͼƬ
	if (pImgPlateClip != NULL && nClipSize > 0)
	{
		VzLPRTcp_ImageSaveToJpeg(pImgPlateClip, nClipSize, "clipImg.jpg", 100);
	}

	// ��ӡ���ƺ�
	printf("plate: %s\n", pResult->license);
	return 0;
}

// ���հ�������ѯ����Ļص�
int __STDCALL gVZLPRC_EMBEB_WLIST_RECV_CALLBACK(VzLPRTcpHandle handle, void *pUserData,
	const VZ_LPR_WLIST_VEHICLE *pWlist, unsigned uCount)
{
	if (pWlist != NULL)
	{
		// ��ӡ��������ѯ����еĳ��ƺ�
		printf("wlist plate: %s\n", pWlist->strPlateID);
	}

	return 0;
}

// ����485���ݵĻص�
void __STDCALL gOnSerialRecvData(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData)
{
	if (pRecvData != NULL)
	{
		const int max_len = 1024;
		char szData[max_len] = { 0 };

		int copy_len = (uRecvSize < (max_len - 1)) ? uRecvSize : max_len - 1;
		memcpy(szData, pRecvData, copy_len);

		// ��ӡһ������͹�����485����
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
		// ���ó���ʶ�����Ļص������ڽ���ʵʱ�ĳ��ƽ����
		VzLPRTcp_SetPlateInfoCallBack(handle, gVZLPRC_TCP_PLATE_INFO_CALLBACK, NULL, TRUE);

		// ���������������
		// ǿ�������ǰ��Ƶ��ʵʱ����������ǰ��Ƶ�������޳��ƣ����Ϊ��_��_���ĳ��ƽ����
		VzLPRTcp_ForceTrigger(handle);

		ret = VzLPRTcp_SetIOOutputAuto(handle, 0, 500);

		int value = -1;
		ret = VzLPRTcp_GetGPIOValue(handle, 0, &value);
		
		// 485���ܲ���
		// �򿪵�һ��485ͨ��
		ret = VzLPRTcp_SerialStart(handle, 0, gOnSerialRecvData, NULL);
		if (ret == 0)
		{
			char szData[] = "123456";

			 //����485����
			 VzLPRTcp_SerialSend(handle, 0, (unsigned char *)szData, 6);
		}

		// ɾ��������
		// ���ַ�����ʾ������а�����
		VzLPRTcp_DeleteWlistVehicle(handle, "");

		// ��Ӱ�����
		VZ_LPR_WLIST_VEHICLE wlist = { 0 };
		strcpy(wlist.strPlateID, "��F12345"); // ���ƺ�
		wlist.bEnable = 1;										// �Ƿ�����
		wlist.bEnableTMEnable = 1;						// �Ƿ�������Чʱ��
		wlist.bEnableTMOverdule = 1;				  // �Ƿ����ù���ʱ��

		// ��Чʱ��
		wlist.struTMEnable.nYear = 2015;
		wlist.struTMEnable.nMonth = 10;
		wlist.struTMEnable.nMDay = 25;
		wlist.struTMEnable.nHour = 12;
		wlist.struTMEnable.nMin = 30;
		wlist.struTMEnable.nSec = 45;

		// ����ʱ��
		wlist.struTMOverdule.nYear = 2015;
		wlist.struTMOverdule.nMonth = 10;
		wlist.struTMOverdule.nMDay = 30;
		wlist.struTMOverdule.nHour = 12;
		wlist.struTMOverdule.nMin = 30;
		wlist.struTMOverdule.nSec = 50;

		// 0��ʾΪ��������1��ʾΪ������
		wlist.bAlarm = 0;

		// �������룬Ҫ�����Ψһ
		strcpy(wlist.strCode, "��F12345");

		// ���������������ð����������򸲸ǣ�����������ӡ�
		VzLPRTcp_ImportWlistVehicle(handle, &wlist);

		// ���ð�������ѯ����Ļص�
		VzLPRTcp_SetWlistInfoCallBack(handle, gVZLPRC_EMBEB_WLIST_RECV_CALLBACK, NULL);

		// ��ѯ����
		// ���ƺ�Ϊ�գ���ʾ��ѯ���а�����
		VzLPRTcp_QueryWlistVehicle(handle, "");

		// ���س����˳�����
		char c = 0;
		while ((c = getchar()) != 0x0a) {
		}

		// �ر�485ͨ��
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