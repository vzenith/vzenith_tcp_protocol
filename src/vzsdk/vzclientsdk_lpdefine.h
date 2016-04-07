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

// 车牌识别相关参数
//---------------------------------------//
//车牌颜色
#define LC_UNKNOWN  0  //未知
#define LC_BLUE     1  //蓝色
#define LC_YELLOW   2  //黄色 
#define LC_WHITE    3  //白色
#define LC_BLACK    4  //黑色
#define LC_GREEN    5  //绿色

//车牌类型
#define LT_UNKNOWN  0   //未知车牌
#define LT_BLUE     1   //蓝牌小汽车
#define LT_BLACK    2   //黑牌小汽车
#define LT_YELLOW   3   //单排黄牌
#define LT_YELLOW2  4   //双排黄牌（大车尾牌，农用车）
#define LT_POLICE   5   //警车车牌
#define LT_ARMPOL   6   //武警车牌
#define LT_INDIVI   7   //个性化车牌
#define LT_ARMY     8   //单排军车牌
#define LT_ARMY2    9   //双排军车牌
#define LT_EMBASSY  10  //使馆车牌
#define LT_HONGKONG 11  //香港进出中国大陆车牌
#define LT_TRACTOR  12  //农用车牌
#define LT_COACH	13  //教练车牌
#define LT_MACAO	14  //澳门进出中国大陆车牌
#define LT_ARMPOL2   15 //双层武警车牌
#define LT_ARMPOL_ZONGDUI 16  // 武警总队车牌
#define LT_ARMPOL2_ZONGDUI 17 // 双层武警总队车牌

//运动方向
#define DIRECTION_LEFT	1  //左
#define DIRECTION_RIGHT	2  //右
#define DIRECTION_UP	3  //上
#define DIRECTION_DOWN	4  //下

//图像格式（TH_SetImageFormat函数的cImageFormat参数）
#define ImageFormatRGB		0			//RGBRGBRGB...
#define ImageFormatBGR		1			//BGRBGRBGR...
#define ImageFormatYUV422	2			//YYYY...UU...VV..	(YV16)
#define ImageFormatYUV420COMPASS 3		//YYYY...UV...		(NV12)
#define ImageFormatYUV420	4			//YYYY...U...V...	(YU12)
#define ImageFormatUYVY	    5			//UYVYUYVYUYVY...	(UYVY)
#define ImageFormatNV21		6			//YYYY...VU...		(NV21)
#define ImageFormatYV12		7			//YYYY...V...U		(YV12)
#define ImageFormatYUYV	    8			//YUYVYUYVYUYV..	(YUYV)

//车辆颜色
#define LGRAY_DARK	0	//深
#define LGRAY_LIGHT	1	//浅

#define LCOLOUR_WHITE	0	//白	
#define LCOLOUR_SILVER	1	//灰(银)
#define LCOLOUR_YELLOW	2	//黄
#define LCOLOUR_PINK	3	//粉
#define LCOLOUR_RED		4	//红
#define LCOLOUR_GREEN	5	//绿
#define LCOLOUR_BLUE	6	//蓝
#define LCOLOUR_BROWN	7	//棕 
#define LCOLOUR_BLACK	8	//黑

// 触发识别类型定义
typedef enum _TH_TRIGGER_TYPE {
    TRIGGER_TYPE_AUTO		=	0,		//自动
    TRIGGER_TYPE_EXTERNAL	=	1,		//外部
    TRIGGER_TYPE_SOFTWARE	=	2,		//软件
    TRIGGER_TYPE_VLOOP					//虚拟地感线圈
} TH_TRIGGER_TYPE;

typedef enum {
    TRIGGER_TYPE_AUTO_BIT		= 1,    //自动
    TRIGGER_TYPE_EXTERNAL_BIT	= 1<<1, //外部
    TRIGGER_TYPE_SOFTWARE_BIT	= 1<<2, //软件
    TRIGGER_TYPE_VLOOP_BIT		= 1<<3  //虚拟地感线圈
} TH_TRIGGER_TYPE_BIT;

/**矩形区域信息*/
typedef struct TH_RECT {
    int left;	/**<左*/
    int top;	/**<上*/
    int right;	/**<右*/
    int bottom;	/**<下*/
} TH_RECT;

typedef struct TH_TimeVal {
    long    tv_sec;         /* 秒 */
    long    tv_usec;        /* 微秒 */
    long	tv_frameStamp;	/* 帧编号 */
} TH_TimeVal;

typedef struct {
    unsigned uTVSec;
    unsigned uTVUSec;
}
VZ_TIMEVAL;

#define TH_LP_LEN	16

/**车牌识别结果信息*/
typedef struct TH_PlateResult {
    char license[TH_LP_LEN];  	/**<车牌号码*/
    char color[8];      		/**<车牌颜色*/
    int nColor;					/**<车牌颜色序号，详见车牌颜色定义LC_X*/
    int nType;					/**<车牌类型，详见车牌类型定义LT_X*/
    int nConfidence;			/**<车牌可信度*/
    int nBright;				/**<亮度评价*/
    int nDirection;				/**<运动方向，详见运动方向定义 DIRECTION_X*/
    TH_RECT rcLocation; 		/**<车牌位置*/
    int nTime;          		/**<识别所用时间*/
    VZ_TIMEVAL tvPTS;			/**<识别时间点*/
    unsigned uBitsTrigType;		/**<强制触发结果的类型,见TH_TRIGGER_TYPE_BIT*/
    unsigned char nCarBright;	/**<车的亮度*/
    unsigned char nCarColor;	/**<车的颜色，详见车辆颜色定义LCOLOUR_X*/
    char reserved0[2];			/**<为了对齐*/
    unsigned uId; 				/**<记录的编号*/
    VzBDTime    struBDTime;     /**<分解时间*/
    char reserved[68];			/**<保留*/
} TH_PlateResult;

#define VZ_LPR_WLIST_LP_MAX_LEN				32
#define VZ_LPR_WLIST_VEHICLE_CODE_LEN		32
#define VZ_LPR_WLIST_VEHICLE_COMMENT_LEN	64

/**日期时间描述*/
typedef struct _VZ_TM {
    short nYear;	/**<年*/
    short nMonth;	/**<月*/
    short nMDay;	/**<日*/
    short nHour;	/**<时*/
    short nMin;		/**<分*/
    short nSec;		/**<秒*/
}
VZ_TM;

/**黑白名单中的车辆记录*/
typedef struct _VZ_LPR_WLIST_VEHICLE {
    unsigned	uVehicleID;										/**<车辆在数据库的ID*/
    char		strPlateID[VZ_LPR_WLIST_LP_MAX_LEN];			/**<车牌字符串*/
    unsigned	uCustomerID;									/**<客户在数据库的ID，与VZ_LPR_WLIST_CUSTOMER::uCustomerID对应*/
    unsigned	bEnable;										/**<该记录有效标记*/
    unsigned	bEnableTMEnable;								/**<是否开启生效时间*/
    unsigned	bEnableTMOverdule;								/**<是否开启过期时间*/
    VZ_TM		struTMEnable;									/**<该记录生效时间*/
    VZ_TM		struTMOverdule;									/**<该记录过期时间*/
    unsigned	bUsingTimeSeg;									/**<是否使用周期时间段*/
    // VZ_TM_PERIOD struTimeSeg;								/**<周期时间段信息*/
    unsigned	bAlarm;											/**<是否触发报警（黑名单记录）*/
    int			iColor;											/**<车辆颜色*/
    int			iPlateType;										/**<车牌类型*/
    char		strCode[VZ_LPR_WLIST_VEHICLE_CODE_LEN];			/**<车辆编码*/
    char		strComment[VZ_LPR_WLIST_VEHICLE_COMMENT_LEN];	/**<车辆编码*/
}
VZ_LPR_WLIST_VEHICLE;


#endif