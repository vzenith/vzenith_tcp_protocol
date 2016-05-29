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
#ifndef _VZ_CLIENT_SDK_LP_DEFINE_H_
#define _VZ_CLIENT_SDK_LP_DEFINE_H_
#include "vzclientsdk_commondefine.h"

// ����ʶ����ز���
//---------------------------------------//
//������ɫ
#define LC_UNKNOWN  0  //δ֪
#define LC_BLUE     1  //��ɫ
#define LC_YELLOW   2  //��ɫ 
#define LC_WHITE    3  //��ɫ
#define LC_BLACK    4  //��ɫ
#define LC_GREEN    5  //��ɫ

//��������
#define LT_UNKNOWN  0   //δ֪����
#define LT_BLUE     1   //����С����
#define LT_BLACK    2   //����С����
#define LT_YELLOW   3   //���Ż���
#define LT_YELLOW2  4   //˫�Ż��ƣ���β�ƣ�ũ�ó���
#define LT_POLICE   5   //��������
#define LT_ARMPOL   6   //�侯����
#define LT_INDIVI   7   //���Ի�����
#define LT_ARMY     8   //���ž�����
#define LT_ARMY2    9   //˫�ž�����
#define LT_EMBASSY  10  //ʹ�ݳ���
#define LT_HONGKONG 11  //��۽����й���½����
#define LT_TRACTOR  12  //ũ�ó���
#define LT_COACH	13  //��������
#define LT_MACAO	14  //���Ž����й���½����
#define LT_ARMPOL2   15 //˫���侯����
#define LT_ARMPOL_ZONGDUI 16  // �侯�ܶӳ���
#define LT_ARMPOL2_ZONGDUI 17 // ˫���侯�ܶӳ���

//�˶�����
#define DIRECTION_LEFT	1  //��
#define DIRECTION_RIGHT	2  //��
#define DIRECTION_UP	3  //��
#define DIRECTION_DOWN	4  //��

//ͼ���ʽ��TH_SetImageFormat������cImageFormat������
#define ImageFormatRGB		0			//RGBRGBRGB...
#define ImageFormatBGR		1			//BGRBGRBGR...
#define ImageFormatYUV422	2			//YYYY...UU...VV..	(YV16)
#define ImageFormatYUV420COMPASS 3		//YYYY...UV...		(NV12)
#define ImageFormatYUV420	4			//YYYY...U...V...	(YU12)
#define ImageFormatUYVY	    5			//UYVYUYVYUYVY...	(UYVY)
#define ImageFormatNV21		6			//YYYY...VU...		(NV21)
#define ImageFormatYV12		7			//YYYY...V...U		(YV12)
#define ImageFormatYUYV	    8			//YUYVYUYVYUYV..	(YUYV)

//������ɫ
#define LGRAY_DARK	0	//��
#define LGRAY_LIGHT	1	//ǳ

#define LCOLOUR_WHITE	0	//��	
#define LCOLOUR_SILVER	1	//��(��)
#define LCOLOUR_YELLOW	2	//��
#define LCOLOUR_PINK	3	//��
#define LCOLOUR_RED		4	//��
#define LCOLOUR_GREEN	5	//��
#define LCOLOUR_BLUE	6	//��
#define LCOLOUR_BROWN	7	//�� 
#define LCOLOUR_BLACK	8	//��

// ����ʶ�����Ͷ���
typedef enum _TH_TRIGGER_TYPE {
    TRIGGER_TYPE_AUTO		=	0,		//�Զ�
    TRIGGER_TYPE_EXTERNAL	=	1,		//�ⲿ
    TRIGGER_TYPE_SOFTWARE	=	2,		//���
    TRIGGER_TYPE_VLOOP					//����ظ���Ȧ
} TH_TRIGGER_TYPE;

typedef enum {
    TRIGGER_TYPE_AUTO_BIT		= 1,    //�Զ�
    TRIGGER_TYPE_EXTERNAL_BIT	= 1<<1, //�ⲿ
    TRIGGER_TYPE_SOFTWARE_BIT	= 1<<2, //���
    TRIGGER_TYPE_VLOOP_BIT		= 1<<3  //����ظ���Ȧ
} TH_TRIGGER_TYPE_BIT;

/**����������Ϣ*/
typedef struct TH_RECT {
    int left;	/**<��*/
    int top;	/**<��*/
    int right;	/**<��*/
    int bottom;	/**<��*/
} TH_RECT;

typedef struct TH_TimeVal {
    long    tv_sec;         /* �� */
    long    tv_usec;        /* ΢�� */
    long	tv_frameStamp;	/* ֡��� */
} TH_TimeVal;

typedef struct {
    unsigned uTVSec;
    unsigned uTVUSec;
}
VZ_TIMEVAL;

#define TH_LP_LEN	16

/**����ʶ������Ϣ*/
typedef struct TH_PlateResult {
    char license[TH_LP_LEN];  	/**<���ƺ���*/
    char color[8];      		/**<������ɫ*/
    int nColor;					/**<������ɫ��ţ����������ɫ����LC_X*/
    int nType;					/**<�������ͣ�����������Ͷ���LT_X*/
    int nConfidence;			/**<���ƿ��Ŷ�*/
    int nBright;				/**<��������*/
    int nDirection;				/**<�˶���������˶������� DIRECTION_X*/
    TH_RECT rcLocation; 		/**<����λ��*/
    int nTime;          		/**<ʶ������ʱ��*/
    VZ_TIMEVAL tvPTS;			/**<ʶ��ʱ���*/
    unsigned uBitsTrigType;		/**<ǿ�ƴ������������,��TH_TRIGGER_TYPE_BIT*/
    unsigned char nCarBright;	/**<��������*/
    unsigned char nCarColor;	/**<������ɫ�����������ɫ����LCOLOUR_X*/
    char reserved0[2];			/**<Ϊ�˶���*/
    unsigned uId; 				/**<��¼�ı��*/
    VzBDTime    struBDTime;     /**<�ֽ�ʱ��*/
    char reserved[68];			/**<����*/
} TH_PlateResult;

#define VZ_LPR_WLIST_LP_MAX_LEN				32
#define VZ_LPR_WLIST_VEHICLE_CODE_LEN		32
#define VZ_LPR_WLIST_VEHICLE_COMMENT_LEN	64

/**����ʱ������*/
typedef struct _VZ_TM {
    short nYear;	/**<��*/
    short nMonth;	/**<��*/
    short nMDay;	/**<��*/
    short nHour;	/**<ʱ*/
    short nMin;		/**<��*/
    short nSec;		/**<��*/
}
VZ_TM;

/**�ڰ������еĳ�����¼*/
typedef struct _VZ_LPR_WLIST_VEHICLE {
    unsigned	uVehicleID;										/**<���������ݿ��ID*/
    char		strPlateID[VZ_LPR_WLIST_LP_MAX_LEN];			/**<�����ַ���*/
    unsigned	uCustomerID;									/**<�ͻ������ݿ��ID����VZ_LPR_WLIST_CUSTOMER::uCustomerID��Ӧ*/
    unsigned	bEnable;										/**<�ü�¼��Ч���*/
    unsigned	bEnableTMEnable;								/**<�Ƿ�����Чʱ��*/
    unsigned	bEnableTMOverdule;								/**<�Ƿ�������ʱ��*/
    VZ_TM		struTMEnable;									/**<�ü�¼��Чʱ��*/
    VZ_TM		struTMOverdule;									/**<�ü�¼����ʱ��*/
    unsigned	bUsingTimeSeg;									/**<�Ƿ�ʹ������ʱ���*/
    // VZ_TM_PERIOD struTimeSeg;								/**<����ʱ�����Ϣ*/
    unsigned	bAlarm;											/**<�Ƿ񴥷���������������¼��*/
    int			iColor;											/**<������ɫ*/
    int			iPlateType;										/**<��������*/
    char		strCode[VZ_LPR_WLIST_VEHICLE_CODE_LEN];			/**<��������*/
    char		strComment[VZ_LPR_WLIST_VEHICLE_COMMENT_LEN];	/**<��������*/
}
VZ_LPR_WLIST_VEHICLE;


#endif