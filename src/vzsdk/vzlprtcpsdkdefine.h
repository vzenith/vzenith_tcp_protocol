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
//VZ����ʶ���豸SDK�Ľṹ��ȶ���
#ifndef _VZ_LPR_CLIENT_SDK_DEFINE_H_
#define _VZ_LPR_CLIENT_SDK_DEFINE_H_

#include "vzsdk/vzclientsdk_commondefine.h"
#include "vzsdk/vzclientsdk_lpdefine.h"

/** LASTERROR ���� */
typedef enum
{
	VZ_LPRC_LASTERROR_SUCCESS,              /**<�����ɹ���� */
	VZ_LPRC_LASTERROR_NOT_SETUP,            /**<δ���� Setup ���� */
	VZ_LPRC_LASTERROR_CREATE_FAILED,        /**<���� LPRC_OBJ ʧ�� */
	VZ_LPRC_LASTERROR_CONNECT_FAILED,       /**<�豸����ʧ�� */
	VZ_LPRC_LASTERROR_INVALID_HANDLE,       /**<LPRC_OBJ ��������� */
	VZ_LPRC_LASTERROR_INVALID_IP,           /**<IP ������ */
	VZ_LPRC_LASTERROR_NOT_FOUND_IP,         /**<δ���ָ� IP */
	VZ_LPRC_LASTERROR_INVALID_PSTATUS,      /**<��ȡ״̬�ı�����ַ������ */
	VZ_LPRC_LASTERROR_REALPLAY_FAILED,      /**<����ʵʱ��Ƶʧ�� */
	VZ_LPRC_LASTERROR_NOT_FOUND_PLAYHANDLE, /**<��Ƶ���ž������ʧ�� */
	VZ_LPRC_LASTERROR_CALLBACK_ERROR,       /**<�ص��������ʹ��� */
	VZ_LPRC_LASTERROR_IMAGEFORMAT_ERROR,    /**<ͼƬ��ʽΪ�� BGR ��ʽ */
	VZ_LPRC_LASTERROR_NULL_IMGINFO,         /**<ͼ��ṹ���ַ������ */
	VZ_LPRC_LASTERROR_WRONG_IMGINFO_DSTBUF, /**<ͼ��ṹ���ַ�����û� JPEG ��Ŀ�Ĵ洢��ַ������ */
	VZ_LPRC_LASTERROR_IOOUTPUT_ERROR,       /**<IO ��ȡ���״̬�ĵ�ַ������ */
	VZ_LPRC_LASTERROR_INVALID_SERIALHANDLE, /**<͸��ͨ����������� */
	VZ_LPRC_LASTERROR_INVALID_WHITELIST,    /**<��������������� */
	VZ_LPRC_LASTERROR_INVALID_FILENAME,     /**<�ļ��������� */
	VZ_LPRC_LASTERROR_SEARCHSET_ERROR,      /**<��ѯ���ƺŷ�ҳ��¼��ֹλ�����ô��� */
	VZ_LPRC_LASTERROR_GETPLATE_ERROR,       /**<��ȡ����ͼƬͼƬ�ڴ��ַ�����û�ͼƬ�ڴ��С���ô����ͼƬ�ڴ��С��ַ������ */
	VZ_LPRC_LASTERROR_PLATESIZE_ERROR,      /**<��Ƶͼ��߶Ȼ��ȱ�����ַ������ */
	VZ_LPRC_LASTERROR_ENCODE_TYPE_ERROR,    /**<��ȡ���뷽ʽ�ĵ�ַ������ */
	VZ_LPRC_LASTERROR_GPIO_STATUS_ERROR,    /**<��ȡ GPIO ״̬�ĵ�ַ������ */
	VZ_LPRC_LASTERROR_GPIO_NUMBER_ERROR,    /**<GPIO ��Ų����� */
	VZ_LPRC_LASTERROR_INVALID_FILEPATH,     /**<�ļ�·�������� */
	VZ_LPRC_LASTERROR_EDITION_ERROR,        /**<��ȡ�����ļ��汾���ڴ��ַ������ */
	VZ_LPRC_LASTERROR_DBERROR_PLATEID_EXISTS,				/**<����ID�Ѿ����� */
	VZ_LPRC_LASTERROR_DBERROR_INSERT_CUSTOMERINFO_FAILED,	/**<��ӿͻ����ϳ��� */
	VZ_LPRC_LASTERROR_DBERROR_INSERT_VEHICLEINFO_FAILED,	/**<��Ӱ��������� */
	VZ_LPRC_LASTERROR_DBERROR_UPDATE_CUSTOMERINFO_FAILED,	/**<���¿ͻ����ϳ��� */
	VZ_LPRC_LASTERROR_DBERROR_UPDATE_VEHICLEINFO_FAILED,	/**<���°��������� */
	VZ_LPRC_LASTERROR_DBERROR_PLATEID_EMPTY,				/**<����������IDΪ�� */
	VZ_LPRC_LASTERROR_DBERROR_ROW_NOT_CHANGED,				/**<����ʧ�� */
	VZ_LPRC_LASTERROR_DBERROR_CUSTOMERINFO_NOT_CHANGED,		/**<�ͻ�����δ���� */
	VZ_LPRC_LASTERROR_DBERROR_VEHICLEINFO_NOT_CHANGED,		/**<����������δ���� */
	VZ_LPRC_LASTERROR_DBERROR_CUSTOMER_VEHICLE_NOT_MATCH,   /**<�ͻ��������������ƥ�� */
	VZ_LPRC_LASTERROR_DBERROR_SQLERRORCODESERVERGONE,		/**<SQLite�������˳��� */
  VZ_LPRC_LASTERROR_GETDEVICE_ERROR,                        /**<��ȡ�豸���кų��� */
  VZ_LPRC_LASTERROR_INVALID_PARAM                   /**<��Ч���� */
}
VZ_LPRC_LASTERROR_TYPE;

/**�豸�������VzLPRTcp_Open��ã��豸������صĺ�������Ҫ*/
typedef int VzLPRTcpHandle;

/**���ûص�����ʱ��Ҫָ��������*/
typedef enum
{
	VZ_LPRC_CALLBACK_COMMON_NOTIFY,	/**<SDKͨ����Ϣ������Ԥ����*/
	VZ_LPRC_CALLBACK_PLATE_STR,		/**<���ƺ����ַ���Ԥ����*/
	VZ_LRPC_CALLBACK_FULL_IMAGE,	/**<����ͼ��*/
	VZ_LPRC_CALLBACK_CLIP_IMAGE,	/**<��ȡͼ��*/
	VZ_LPRC_CALLBACK_PLATE_RESULT,	/**<ʵʱʶ����*/
	VZ_LPRC_CALLBACK_PLATE_RESULT_STABLE,	/**<�ȶ�ʶ����*/
	VZ_LPRC_CALLBACK_PLATE_RESULT_TRIGGER,	/**<������ʶ����������API���������IO��Ӳ������ʽ��*/
	VZ_LPRC_CALLBACK_VIDEO,			/**<��Ƶ֡�ص�*/
}
VZ_LPRC_CALLBACK_TYPE;

/**ͨ����Ϣ��������*/
typedef enum
{
	VZ_LPRC_NO_ERR = 0,		/**<�޴���*/
	VZ_LPRC_ACCESS_DENIED,	/**<�û����������*/
	VZ_LPRC_NETWORK_ERR,	/**<�������ӹ���*/
    VZ_LPRC_ONLINE,         /**<�豸����*/
    VZ_LPRC_OFFLINE,        /**<�豸����*/
	VZ_LPRC_IO_INPUT = 100, /**<IO�������ź�*/
}
VZ_LPRC_COMMON_NOTIFY;

/**ʶ����������*/
typedef enum
{
	VZ_LPRC_RESULT_REALTIME,		/**<ʵʱʶ����*/
	VZ_LPRC_RESULT_STABLE,			/**<�ȶ�ʶ����*/
	VZ_LPRC_RESULT_FORCE_TRIGGER,	/**<���á�VzLPRClient_ForceTrigger��������ʶ����*/
	VZ_LPRC_RESULT_IO_TRIGGER,		/**<�ⲿIO�źŴ�����ʶ����*/
	VZ_LPRC_RESULT_VLOOP_TRIGGER,	/**<������Ȧ������ʶ����*/
	VZ_LPRC_RESULT_MULTI_TRIGGER,	/**<��_FORCE_\_IO_\_VLOOP_�е�һ�ֻ����ͬʱ������������Ҫ����ÿ��ʶ������TH_PlateResult::uBitsTrigType���ж�*/
	VZ_LPRC_RESULT_TYPE_NUM			/**<����������*/
}
VZ_LPRC_RESULT_TYPE;

/**���㶨��
  *X_1000��Y_1000��ȡֵ��ΧΪ[0, 1000]��
  *��λ����ϢΪʵ��ͼ��λ��������ͼ��λ�õ���Գߴ磻
  *����X_1000 = x*1000/win_width������xΪ����ͼ���е�ˮƽ����λ�ã�win_widthΪͼ����
*/
typedef struct
{
	unsigned X_1000;	/**<ˮƽ�����������*/
	unsigned Y_1000;	/**<��ֱ�����������*/
}
VZ_LPRC_VERTEX;

/**��Խ����ö��*/
typedef enum
{
	VZ_LPRC_BIDIR,		/**˫�򴥷�*/
	VZ_LPRC_UP2DOWN,	/**���ϵ��·���ĳ�������*/
	VZ_LPRC_DOWN2UP,	/**���µ��Ϸ���ĳ�������*/
}
VZ_LPRC_DIR;
/**������Ȧ����*/
#define VZ_LPRC_VIRTUAL_LOOP_NAME_LEN	32

/**������Ȧ�������*/
#define VZ_LPRC_VIRTUAL_LOOP_VERTEX_NUM	4

/**������Ȧ��Ϣ����*/
typedef struct
{
	BYTE			byID;		/**<���*/
	BYTE			byEnable;	/**<�Ƿ���Ч*/
	BYTE			byDraw;		/**<�Ƿ����*/
	BYTE			byRes[1];	/**<Ԥ��*/
	char			strName[VZ_LPRC_VIRTUAL_LOOP_NAME_LEN];			/**<�Զ�������*/
	VZ_LPRC_VERTEX	struVertex[VZ_LPRC_VIRTUAL_LOOP_VERTEX_NUM];	/**<��������*/
	VZ_LPRC_DIR		eCrossDir;	/**<��Խ��������*/
	unsigned		uTriggerTimeGap;	/**����ͬ���ƵĴ���ʱ���������ƣ���λΪ��*/
	unsigned		uMaxLPWidth;		/**����Ƴߴ�����*/
	unsigned		uMinLPWidth;		/**��С���Ƴߴ�����*/
}
VZ_LPRC_VIRTUAL_LOOP;

/**���������Ȧ����*/
#define VZ_LPRC_VIRTUAL_LOOP_MAX_NUM	8

/**������Ȧ����*/
typedef struct
{
	unsigned				uNumVirtualLoop;	/**<ʵ�ʸ���*/
	VZ_LPRC_VIRTUAL_LOOP	struLoop[VZ_LPRC_VIRTUAL_LOOP_MAX_NUM];	/**<������Ȧ����*/
}
VZ_LPRC_VIRTUAL_LOOPS;

/**ʶ�����򶥵����*/
#define VZ_LPRC_ROI_VERTEX_NUM	4

/**ʶ��������Ϣ����*/
typedef struct
{
    BYTE            byRes1;     /**<Ԥ��*/   
    BYTE            byEnable;   /**<�Ƿ���Ч*/
    BYTE            byDraw;     /**<�Ƿ����*/
    BYTE            byRes2;     /**<Ԥ��*/
    BYTE            byRes3[32]; /**<Ԥ��*/
    VZ_LPRC_VERTEX  struVertex[VZ_LPRC_ROI_VERTEX_NUM]; /**<��������*/
}
VZ_LPRC_ROI;

/**ʡ���ַ��������ߴ�*/
#define VZ_LPRC_PROVINCE_STR_LEN	128

/**Ԥ��ʡ����Ϣ*/
typedef struct
{
	char strProvinces[VZ_LPRC_PROVINCE_STR_LEN];	/**<����֧�ֵ�ʡ�ݼ�ƹ��ɵ��ַ���*/
	int nCurrIndex;	/**<��ǰ��Ԥ��ʡ�ݵ���ţ���strProvinces�еģ�-1Ϊδ����*/
}
VZ_LPRC_PROVINCE_INFO;

/**ͼ����Ϣ*/
typedef struct
{
	unsigned		uWidth;			/**<���*/
	unsigned		uHeight;		/**<�߶�*/
	unsigned		uPitch;			/**<ͼ���ȵ�һ��������ռ�ڴ��ֽ���*/
	unsigned		uPixFmt;		/**<ͼ�����ظ�ʽ���ο�ö�ٶ���ͼ���ʽ��ImageFormatXXX��*/
	unsigned char	*pBuffer;	/**<ͼ���ڴ���׵�ַ*/
}
VZ_LPRC_IMAGE_INFO;

/**����֡����*/
typedef enum
{
	VZ_LPRC_DATA_ENC_VIDEO,		/**<��Ƶ֡��������*/
	VZ_LPRC_DATA_ENC_AUDIO,		/**<��Ƶ֡��������*/
}
VZ_LPRC_DATA_TYPE;

/**����֡��Ϣ*/
typedef struct
{
	unsigned char	*pBuffer;	/**<�����ڴ���׵�ַ*/
	unsigned		uDataSize;	/**<����֡�ߴ磬�ֽ���*/
	unsigned		uPTS_Sec;	/**<֡ʱ���_��*/
	unsigned		uPTS_USec;	/**<֡ʱ���_΢��*/
	unsigned char	uIsKeyFrame;/**<�ؼ�֡��־λ��1Ϊ�ؼ�֡��0Ϊ�ǹؼ�֡*/
	unsigned char	reserved[7];	/**<Ԥ��*/
}
VZ_LPRC_DATA_INFO;

/**��ͷ�䱶�佹����*/
typedef enum
{
	VZ_LENS_OPT_STOP,		/**<ֹͣ����*/
	VZ_LENS_FOCUS_FAR,		/**<��Զ���۽�*/
	VZ_LENS_FOCUS_NEAR,		/**<�������۽�*/
	VZ_LENS_ZOOM_TELE,		/**<�������䱶*/
	VZ_LENS_ZOOM_WIDE,		/**<���̽��䱶*/
}
VZ_LENS_OPT;

/**LED���������*/
typedef enum
{
	VZ_LED_AUTO,		/**<�Զ�����LED�Ŀ��͹�*/
	VZ_LED_MANUAL_ON,	/**<�ֶ�����LED����*/
	VZ_LED_MANUAL_OFF,	/**<�ֶ�����LED�ر�*/
}
VZ_LED_CTRL;

/**�û�˽�����ݵ�����ֽ���*/
#define VZ_LPRC_USER_DATA_MAX_LEN	128

/**�豸Ψһ���к�*/
typedef struct
{
	unsigned uHi;	/**<��λ*/
	unsigned uLo;	/**<��λ*/
}
VZ_DEV_SERIAL_NUM;

/**�豸����ʱ�����*/
typedef struct
{
	unsigned uYear;		/**<��*/
	unsigned uMonth;	/**<�� [1, 12]*/
	unsigned uMDay;		/**<���е����� [1, 31]*/
	unsigned uHour;		/**<ʱ*/
	unsigned uMin;		/**<��*/
	unsigned uSec;		/**<��*/
}
VZ_DATE_TIME_INFO;

/**���ڲ���*/
typedef struct
{
	unsigned uBaudRate;		// <������ 300,600,1200,2400,4800,9600,19200,34800,57600,115200
	unsigned uParity;		// <У��λ ��ֵΪ0-2=no,odd,even
	unsigned uDataBits;		// <����λ ��ֵΪ7,8 λ����λ
	unsigned uStopBit;		// <ֹͣλ ��ֵΪ1,2λֹͣλ
}
VZ_SERIAL_PARAMETER;

/**�洢�豸����*/
typedef enum
{
	VZ_STORAGE_DEV_SD = 0,	/**<SD��*/
	VZ_STORAGE_DEV_HD		/**<Ӳ��*/
}
VZ_STORAGE_DEV_TYPE;

/**�洢�豸״̬*/
typedef enum
{
	VZ_STORAGE_DEV_NO_PART = 0, /**<δ����*/
	VZ_STORAGE_DEV_PART_OK,		/**<�ѷ���*/
}
VZ_STORAGE_DEV_STATUS;

/**�洢�豸�����Ĺ���״̬*/
typedef enum
{
	VZ_STORAGE_DEV_PART_ERROR = 0,	/**<�쳣*/
	VZ_STORAGE_DEV_PART_NOT_FORMAT,	/**<δ��ʽ��*/
	VZ_STORAGE_DEV_PART_NOT_MOUNT,	/**<δ����*/
	VZ_STORAGE_DEV_PART_FORMATING,	/**<���ڸ�ʽ��*/
	VZ_STORAGE_DEV_PART_DELETING,	/**<����ɾ���ļ�*/
	VZ_STORAGE_DEV_PART_NORMAL,		/**<��������״̬*/
}
VZ_STORAGE_DEV_PART_STATUS;

/**�洢�豸������Ϣ*/
typedef struct
{
	VZ_STORAGE_DEV_PART_STATUS	eStatus;	/**<����״̬*/
	unsigned	uUsed;			/**<���ÿռ䣨MB)*/
	unsigned	uLeft;			/**<���ÿռ䣨MB)*/
	unsigned	uTotal;			/**<��������MB��*/
	int			nFormatPercent;	/**<��ʽ���ٷֱ�*/
}
VZ_STORAGE_DEV_PART_INFO;

#define VZ_MAX_NUM_STORAGE_DEV_PART_NUM	4
/**�����洢�豸��Ϣ*/
typedef struct
{
	char reserved[32];
	VZ_STORAGE_DEV_TYPE		eType;					/**<�洢�豸����*/
	VZ_STORAGE_DEV_STATUS	eStatus;				/**<�洢�豸״̬*/
	unsigned				uPartNum;				/**<��������*/
	VZ_STORAGE_DEV_PART_INFO	struPartInfo[VZ_MAX_NUM_STORAGE_DEV_PART_NUM];		/**<��������Ϣ*/
}
VZ_STORAGE_DEVICE_INFO;

#define VZ_MAX_NUM_STORAGE_DEVICE	4
/**�洢�豸��Ϣ*/
typedef struct
{
	int nDevCount;		/**<�洢�豸����*/
	VZ_STORAGE_DEVICE_INFO	struSDI[VZ_MAX_NUM_STORAGE_DEVICE];	/**<���洢�豸��Ϣ*/
}
VZ_STORAGE_DEVICES_INFO;

#define LPRC_OSDTEXTLEN				16

typedef struct
{
	unsigned char	dstampenable;					// 0 off 1 on
	int				dateFormat;						// 0:YYYY/MM/DD;1:MM/DD/YYYY;2:DD/MM/YYYY
	int				datePosX;
	int				datePosY;
	unsigned char	tstampenable;   				// 0 off 1 on
	int				timeFormat;						// 0:12Hrs;1:24Hrs
	int				timePosX;
	int				timePosY;
	unsigned char	nLogoEnable;					// 0 off 1 on
	int				nLogoPositionX;   				///<  logo position
	int				nLogoPositionY;   				///<  logo position
	unsigned char	nTextEnable;					///0 off 1 on
	int				nTextPositionX;   				///<  text position
	int				nTextPositionY;   				///<  text position
	char			overlaytext[LPRC_OSDTEXTLEN];	///user define text
}VZ_LPRC_OSD_Param;

#define LPRC_IPLEN				16

typedef struct VZ_LPRC_NETCFG
{
	char ipaddr[LPRC_IPLEN];
	char netmask[LPRC_IPLEN];
	char gateway[LPRC_IPLEN];
	char dns[LPRC_IPLEN];
	WORD  http_port;
}VZ_LPRC_NETCFG;


/**�����õ�ʶ������*/
#define VZ_LPRC_REC_BLUE	    (1<<(LT_BLUE))						/**<���Ƴ�*/
#define VZ_LPRC_REC_YELLOW	    (1<<(LT_YELLOW)|1<<(LT_YELLOW2))	/**<���Ƴ�*/
#define VZ_LPRC_REC_BLACK	    (1<<(LT_BLACK))						/**<���Ƴ�*/
#define VZ_LPRC_REC_COACH	    (1<<(LT_COACH))						/**<������*/
#define VZ_LPRC_REC_POLICE	    (1<<(LT_POLICE))					/**<����*/
#define VZ_LPRC_REC_AMPOL	    (1<<(LT_ARMPOL))					/**<�侯��*/
#define VZ_LPRC_REC_ARMY        (1<<(LT_ARMY)|1<<(LT_ARMY2))		/**<����*/
#define VZ_LPRC_REC_GANGAO	    (1<<(LT_HONGKONG)|1<<(LT_MACAO))	/**<�۰Ľ�����½��*/
#define VZ_LPRC_REC_EMBASSY     (1<<(LT_EMBASSY))                   /**<ʹ�ݳ�*/

#endif
