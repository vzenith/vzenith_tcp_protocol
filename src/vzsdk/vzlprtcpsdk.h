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
#ifndef _VZ_LPRC_TCP_SDK_H_
#define _VZ_LPRC_TCP_SDK_H_
#include "vzsdk/vzlprtcpsdkdefine.h"

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
* @defgroup group_global ȫ�ֲ�������
* @defgroup group_device �����豸��������
* @defgroup group_callback �ص�����
*/

/**
*  @brief ȫ�ֳ�ʼ��
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note �����нӿڵ���֮ǰ����
*  @ingroup group_global
*/
extern "C" int __STDCALL VzLPRTcp_Setup();

/**
*  @brief ȫ���ͷ�
*  @note �ڳ������ʱ���ã��ͷ�SDK����Դ
*  @ingroup group_global
*/
VZ_LPRC_TCP_API void __STDCALL VzLPRTcp_Cleanup();

/**
*  @brief ��һ���豸
*  @param  [IN] pStrIP �豸��IP��ַ
*  @param  [IN] wPort �豸�Ķ˿ں�
*  @param  [IN] pStrUserName �����豸�����û���
*  @param  [IN] pStrPassword �����豸��������
*  @return �����豸�Ĳ������������ʧ��ʱ������0
*  @ingroup group_device
*/
VZ_LPRC_TCP_API VzLPRTcpHandle __STDCALL VzLPRTcp_Open(const char *pStrIP, WORD wPort, const char *pStrUserName, const char *pStrPassword);

/**
*  @brief �ر�һ���豸
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_Close(VzLPRTcpHandle handle);

/**
*  @brief ͨ��IP��ַ�ر�һ���豸
*  @param  [IN] pStrIP �豸��IP��ַ
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_CloseByIP(const char *pStrIP);

/**
*  @brief ��ȡ�豸���к�
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [OUT] pSN �豸���к�
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetDeviceSN(VzLPRTcpHandle handle, char* pSN);

/**
*  @brief ��ȡ����״̬
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN/OUT] pStatus �����ȡ״̬�ı�����ַ���������Ϊ 1�����ϣ�0δ����
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   �û��������ڵ��øú�����������ѯ�豸�Ƿ���ߣ��Լ����ߺ��Ƿ�ָ�����
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_IsConnected(VzLPRTcpHandle handle, BYTE *pStatus);

/**
*  @brief ͨ���ûص�������ó���ʶ����Ϣ
*  @param  [OUT] handle			��VzLPRTcp_Open������õľ��
*  @param  [OUT] pUserData		�ص�������������
*  @param  [OUT] pResult		������Ϣ�����׵�ַ������ṹ�嶨�� TH_PlateResult
*  @param  [OUT] uNumPlates		���������еĳ��Ƹ���
*  @param  [OUT] eResultType	����ʶ�������ͣ����ö�����Ͷ���VZ_LPRC_RESULT_TYPE
*  @param  [OUT] pImgFull		��ǰ֡��ͼ�������׵�ַ
*  @param  [OUT] nFullSize		ͼ�����ڴ��еĴ�С
*  @param  [OUT] pImgPlateClip	��ǰ֡�г��������ͼ����������
*  @param  [OUT] nClipSize		��������ͼ�����ڴ��еĴ�С
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   �����Ҫ�ûص��������ؽ�ͼ���� pImgFull ��pImgPlateClip����Ҫ�����ûص�������VzLPRTcp_SetPlateInfoCallBack��ʱ�����ͼ���ݣ�����ûص��������ص�������ָ��Ϊ�գ�
*  @ingroup group_callback
*/
typedef int (__STDCALL *VZLPRC_TCP_PLATE_INFO_CALLBACK)(VzLPRTcpHandle handle, void *pUserData,
													const TH_PlateResult *pResult, unsigned uNumPlates,
													VZ_LPRC_RESULT_TYPE eResultType,
													unsigned char *pImgFull, int nFullSize, 
													unsigned char *pImgPlateClip, int nClipSize);

/**
*  @brief ����ʶ�����Ļص�����
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] func ʶ�����ص����������ΪNULL�����ʾ�رոûص������Ĺ���
*  @param  [IN] pUserData �ص������е�������
*  @param  [IN] bEnableImage ָ��ʶ�����Ļص��Ƿ���Ҫ������ͼ��Ϣ��1Ϊ��Ҫ��0Ϊ����Ҫ
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetPlateInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_PLATE_INFO_CALLBACK func, void *pUserData, int bEnableImage);


/**
*  @brief ��ͼ�񱣴�ΪJPEG��ָ��·��
*  @param  [IN] pImgBuf  ͼ���ڴ��׵�ַ
*  @param  [IN] nImgSize ͼ���ڴ��С
*  @param  [IN] pSavePathName �������·����JPG��׺�����ļ����ַ���
*  @param  [IN] nQuality��Ԥ���� JPEGѹ����������ȡֵ��Χ1~100��
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   �������ļ����е�·����Ҫ����
*  @ingroup group_global
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ImageSaveToJpeg(unsigned char *pImgBuf, int nImgSize, const char *pSavePathName, int nQuality);

/**
*  @brief ͨ���ûص��������͸��ͨ�����յ�����
*  @param  [IN] nSerialHandle VzLPRTcp_SerialStart���صľ��
*  @param  [IN] pRecvData	���յ����ݵ��׵�ַ
*  @param  [IN] uRecvSize	���յ����ݵĳߴ�
*  @param  [IN] pUserData	�ص�����������
*  @ingroup group_callback
*/
typedef void (__STDCALL *VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK)(int nSerialHandle, const unsigned char *pRecvData, unsigned uRecvSize, void *pUserData);

/**
*  @brief ����͸��ͨ��
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] nSerialPort ָ��ʹ���豸�Ĵ�����ţ�0��ʾ��һ�����ڣ�1��ʾ�ڶ�������
*  @param  [IN] func �������ݵĻص�����
*  @param  [IN] pUserData �������ݻص�������������
*  @return 0��ʾ�ɹ�������ֵ��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialStart(VzLPRTcpHandle handle, int nSerialPort,
												  VZDEV_TCP_SERIAL_RECV_DATA_CALLBACK func, void *pUserData);
/**
*  @brief ͸��ͨ����������
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] nSerialPort ָ��ʹ���豸�Ĵ�����ţ�0��ʾ��һ�����ڣ�1��ʾ�ڶ�������
*  @param [IN] pData ��Ҫ��������ݿ���׵�ַ
*  @param [IN] uSizeData ��Ҫ��������ݿ���ֽ���
*  @return 0��ʾ�ɹ�������ֵ��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialSend(VzLPRTcpHandle handle, int nSerialPort, const unsigned char *pData, unsigned uSizeData);

/**
*  @brief ͸��ͨ��ֹͣ��������
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @return 0��ʾ�ɹ�������ֵ��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SerialStop(VzLPRTcpHandle handle);

/**
*  @brief ����IO��������Զ���λ
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] uChnId IO�����ͨ���ţ���0��ʼ
*  @param  [IN] nDuration ��ʱʱ�䣬ȡֵ��Χ[500, 5000]����
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetIOOutputAuto(VzLPRTcpHandle handle, unsigned uChnId, int nDuration);


/**
*  @brief ����IO�����״̬
*  @param  [IN] handle ��VzLPRClient_Open������õľ��
*  @param  [IN] uChnId IO�����ͨ���ţ���0��ʼ
*  @param  [OUT] nOutput ��Ҫ���õ�IO�����״̬��0��ʾ�̵�����·��1��ʾ�̵�����·
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetIOOutput(VzLPRTcpHandle handle, unsigned uChnId, int nOutput);

/**
*  @brief ͨ���ûص���������豸��һ��״̬��Ϣ
*  @param  [IN] handle		��VzLPRTcp_Open������õľ��
*  @param  [IN] pUserData	�ص�����������
*  @param  [IN] eNotify		ͨ����Ϣ��������
*  @param  [IN] pStrDetail	��ϸ�����ַ���
*  @ingroup group_callback
*/
typedef void (__STDCALL *VZLPRC_TCP_COMMON_NOTIFY_CALLBACK)(VzLPRTcpHandle handle
                                                            , void *pUserData
                                                            , VZ_LPRC_COMMON_NOTIFY eNotify
                                                            , const char *pStrDetail);

/**
*  @brief �����豸���ӷ��������صĻص�����
*  @param  [IN] func �豸���ӽ����״̬��ͨ���ûص���������
*  @param  [IN] pUserData �ص������е�������
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_global
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetCommonNotifyCallBack(VZLPRC_TCP_COMMON_NOTIFY_CALLBACK func, void *pUserData);

/**
*  @brief ����ID��ȡ����ͼƬ
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] id     ���Ƽ�¼��ID
*  @param  [IN] pdata  �洢ͼƬ���ڴ�
*  @param  [IN][OUT] size Ϊ���봫��ֵ������ΪͼƬ�ڴ�Ĵ�С�����ص��ǻ�ȡ��jpgͼƬ�ڴ�Ĵ�С
*  @return ����ֵΪ0��ʾ�ɹ�������-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_LoadImageById(VzLPRTcpHandle handle, int id, void *pdata, int* size);

/**
*  @brief ��ȡGPIO��״̬
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] gpioIn ����Ϊ0��1
*  @param  [OUT] value 0�����·��1����·
*  @return ����ֵΪ0��ʾ�ɹ�������-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetGPIOValue(VzLPRTcpHandle handle, int gpioIn, int *value);

/**
*  @brief �����ѻ���飬ʵ���ѻ��Զ��л��������Ĺ��ܡ�
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] pUserData �������ݻص�������������
*  @note   ��Чǰ�᣺�豸�ġ����������ҳ��ġ���������֤��TABҳ�еġ���������������������Ϊ���ѻ��Զ����á�
*  @note   ���øýӿڣ��豸����øýӿڵĿͻ���֮��ͻ��γɰ󶨣�����ͻ������豸֮��������������ô�豸�ϵİ�������Ϊͣ��״̬���������IO�źſ��Ƶ�բ̧�˺����485����Э�飻
*  @note   ����ͻ������豸�豸֮���������ˣ����߿ͻ��˹رգ����ߵ����˹ر��ѻ���飨CancelOfflineCheck���ӿڣ���ô�豸�ϵİ����������ã��豸���ݰ��������Ƶ�բ̧�˺����485����Э�飻
*  @return ����ֵΪ0��ʾ�ɹ�������-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetOfflineCheck(VzLPRTcpHandle handle);

/**
*  @brief �ر��ѻ����
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] pUserData �������ݻص�������������
*  @note   ��Чǰ�᣺�豸�ġ����������ҳ��ġ���������֤��TABҳ�еġ���������������������Ϊ���ѻ��Զ����á�
*  @note   �ر��ѻ���飬�豸���ڰ����������IO�źſ��Ƶ�բ̧�˺����485����Э�顣
*  @return ����ֵΪ0��ʾ�ɹ�������-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_CancelOfflineCheck(VzLPRTcpHandle handle);

/**
*  @brief ������������źţ�ǿ�ƴ���ǰʱ�̵����ݲ�������
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   ����ʶ����ͨ���ص������ķ�ʽ���أ������ǰ��Ƶ�������޳��ƣ���ص��������ᱻ����
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ForceTrigger(VzLPRTcpHandle handle);

/**
* @brief ��ȡ LastError
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_GetLastError( );


/**
*  @brief ����ID��ȡ���Ƽ�¼
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] id     ���Ƽ�¼��ID
*  @param  [IN] pResult���Ƽ�¼
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   ����ʶ����ͨ���ص������ķ�ʽ���أ������ǰ��Ƶ�������޳��ƣ���ص��������ᱻ����
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_LoadRecordById(VzLPRTcpHandle handle, int id, TH_PlateResult *pResult);

/**
*  @brief ��ȡ�豸��IP
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @return �����豸��IP
*  @ingroup group_device
*/
VZ_LPRC_TCP_API const char* __STDCALL VzLPRTcp_GetIP(VzLPRTcpHandle handle);

/**
*  @brief ����һ����������¼
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] pLprWlistVehicle ��������¼
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   �����������¼�Ѿ����ڣ�����¡������ڣ�����ӵ����������ݿ��С�
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_ImportWlistVehicle(VzLPRTcpHandle handle, VZ_LPR_WLIST_VEHICLE *pWlist);

/**
*  @brief ���ݳ��ƺ�ɾ��������
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] plateCode ���ƺ�
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_DeleteWlistVehicle(VzLPRTcpHandle handle, const char* plateCode);

/**
*  @brief ���ݳ��ƺŲ�ѯ������
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] plateCode ���ƺ�
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @note   ������ͨ���ص���������ƺ�Ϊ�գ��������еİ�������
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_QueryWlistVehicle(VzLPRTcpHandle handle, const char* plateCode);

/**
*  @brief ͨ���ûص�������ð�������ѯ���
*  @param  [IN] handle			��VzLPRTcp_Open������õľ��
*  @param  [IN] pUserData		�ص�������������
*  @param  [IN] pWlist			�����������׵�ַ������ṹ�嶨�� VZ_LPR_WLIST_VEHICLE
*  @param  [IN] uCount			�����������еĳ��Ƹ���
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_callback
*/
typedef int (__STDCALL *VZLPRC_TCP_WLIST_RECV_CALLBACK)(VzLPRTcpHandle handle, void *pUserData,
														  const VZ_LPR_WLIST_VEHICLE *pWlist, unsigned uCount);

/**
*  @brief ���ð���������Ļص�����
*  @param  [IN] handle ��VzLPRTcp_Open������õľ��
*  @param  [IN] func �������ص����������ΪNULL�����ʾ�رոûص������Ĺ���
*  @param  [IN] pUserData �ص������е�������
*  @return 0��ʾ�ɹ���-1��ʾʧ��
*  @ingroup group_device
*/
VZ_LPRC_TCP_API int __STDCALL VzLPRTcp_SetWlistInfoCallBack(VzLPRTcpHandle handle, VZLPRC_TCP_WLIST_RECV_CALLBACK func, void *pUserData);

#ifdef __cplusplus
}
#endif

#endif