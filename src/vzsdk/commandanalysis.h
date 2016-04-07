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
#ifndef SRC_HSHA_COMMANDANALYSIS_H_
#define SRC_HSHA_COMMANDANALYSIS_H_

#include <string>
#include "json\json.h"
#include "vzclientsdk_lpdefine.h"
#include "vzsdkdefines.h"

using namespace std;

typedef struct _TTRANSMISSION_RESPONSE
{
  const char*	subcmd;
  const char*	response;
  unsigned int datalen;	// 数据长度
  unsigned int comm;		// 串口号
  const char* data;		// 串口数据
}
TTRANSMISSION_RESPONSE;

typedef struct _OFFLINE_RESPONSE
{
  const char*	subcmd;
  const char*	response;
}OFFLINE_RESPONSE;

typedef struct _RECORD_RESPONSE
{
  unsigned int id;
  unsigned int size;
}RECORD_RESPONSE;

typedef struct _GPIO_RESPONSE
{
  unsigned int gpio;
  unsigned int val;
}GPIO_RESPONSE;

typedef struct _MAX_REC_RESPONSE
{
  unsigned int max_id;
}MAX_REC_RESPONSE;


typedef struct _TIMESTAMP_RESPONSE
{
  unsigned int timestamp;
}TIMESTAMP_RESPONSE;

typedef struct _WHITE_LIST_OPERATOR_RESPONSE
{
  const char* operator_type;
  const char* state;
  unsigned int record_count;

}WHITE_LIST_OPERATOR_RESPONSE;

class commandanalysis
{
public:
  commandanalysis(void);
  ~commandanalysis(void);

  static string GeneratCommonCmd(const char *command);
  static Json::Value GeneratSerialStartCmd(uint32 serial_port);
  static Json::Value GeneratSerialStopCmd();
  static Json::Value GeneratSerialSendCmd(uint32 serial_port, const unsigned char* data, int datalen);
  static void GeneratSetOfflineCheckCmd(unsigned int interval, Json::Value& _json_value);
  static void GeneratCancelOfflineCheckCmd(Json::Value& _json_value);
  static void GeneratGetImageByIdCmd(int id, Json::Value& _json_value);
  static void GeneratGetRecordByIdCmd(int id, bool needImg, Json::Value& _json_value);
  static Json::Value GeneratGetGPIOValueCmd(int gpio);
  static Json::Value GeneratSetGPIOAutoCmd(int gpio, int duration);
  static Json::Value GeneratImportWlistVehicleCmd(const VZ_LPR_WLIST_VEHICLE *item);
  static Json::Value GeneratDeleteWlistVehicleCmd(const char* plate_code);
  static Json::Value GeneratQueryWlistVehicleCmd(const char* plate_code);
  static Json::Value GeneratTTransmissionCmd(const char *comm, const char* data, int datalen);
  static Json::Value GeneratOfflineCheckCmd(unsigned int interval);

  //mainten
  static void GeneratGetDeviceSN(int _session_id, Json::Value& _json_value);

  static std::string IvsFormatToString(int ivs_format);
  //Recongnition
  static void GeneratIVSResult(bool _enable_result
    , int _format
    , bool _enable_img
    , int _img_type
    , Json::Value& _json_value);

  static void GeneratForceTrigger(Json::Value& _json_value);

  static void ParseTTransmissionResponse(Json::Value &root, TTRANSMISSION_RESPONSE *value);
  static void ParseOfflineResponse(Json::Value &root, OFFLINE_RESPONSE *value);
  static void ParseRecordResponse(Json::Value &root, RECORD_RESPONSE *value);
  static void ParseGPIOResponse(Json::Value &root, GPIO_RESPONSE *value);
  static void ParseMaxRecResponse(Json::Value &root, MAX_REC_RESPONSE *value);
  static void ParseTimestampResponse(Json::Value &root, TIMESTAMP_RESPONSE *value);
  static void ParseWhiteListOperatorResponse(Json::Value &root, WHITE_LIST_OPERATOR_RESPONSE *value);
  static void ParseWhiteListRecordResponse(Json::Value &item, VZ_LPR_WLIST_VEHICLE &wlist);
  static void ParsePlateResultResponse(Json::Value &root, TH_PlateResult &result, int &fullImgSize, int &clipImgSize);
};

#endif