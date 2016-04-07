#include "commandanalysis.h"
#ifndef _WIN32
#include <string.h>
#endif
#include "base/base64.h"
#include "vzsdkbase.h"


commandanalysis::commandanalysis(void) {
}

commandanalysis::~commandanalysis(void) {
}

string commandanalysis::GeneratCommonCmd( const char *command ) {
    string cmd;

    Json::Value root;

    root["cmd"]		= command;

    cmd = root.toStyledString();
    return cmd;
}

Json::Value commandanalysis::GeneratSerialStartCmd(uint32 serial_port) {
    Json::Value root;

    if (serial_port != 0 || serial_port != 1) {
        return root;
    }

    root["cmd"]		= "ttransmission";
    root["subcmd"]  = "init";
    if (serial_port == 0) {
        root["data"]	= "rs485-1";
        root["datalen"] = strlen("rs485-1");
    } else if (serial_port == 1) {
        root["data"] = "rs485-2";
        root["datalen"] = strlen("rs485-2");
    }

    return root;
}

Json::Value commandanalysis::GeneratSerialStopCmd() {
    string cmd;
    Json::Value root;
    root["cmd"]		= "ttransmission";
    root["subcmd"]  = "uninit";

    cmd = root.toStyledString();
    return cmd;
}

Json::Value commandanalysis::GeneratSerialSendCmd(uint32 serial_port, const unsigned char* data, int datalen) {
    string cmd;

    Json::Value root;

    root["cmd"]		= "ttransmission";
    root["subcmd"]  = "send";
    if (serial_port == 0) {
        root["comm"] = "rs485-1";
    } else if (serial_port == 1) {
        root["comm"] = "rs485-2";
    }

    string result;
    vzsdk::Base64::EncodeFromArray(data, datalen, &result);

    root["data"]	= result;
    root["datalen"] = datalen;

    return root;
}

void commandanalysis::GeneratSetOfflineCheckCmd(unsigned int interval, Json::Value& _json_value) {
    _json_value["cmd"]		= "offline";
    _json_value["interval"] = interval;
}

void commandanalysis::GeneratCancelOfflineCheckCmd(Json::Value& _json_value) {
    _json_value["cmd"]		= "offline";
    _json_value["subcmd"]	= "cancel";
}

void commandanalysis::GeneratGetImageByIdCmd(int id, Json::Value& _json_value) {
    _json_value["cmd"]  = "get_image";
    _json_value["id"]   = id;
}

void commandanalysis::GeneratGetRecordByIdCmd(int id, bool needImg, Json::Value& _json_value) {
    _json_value["cmd"]		= "get_record";
    _json_value["id"]		= id;
    _json_value["format"]	= "json";
    _json_value["image"]	= needImg ? true : false;
}

Json::Value commandanalysis::GeneratOfflineCheckCmd(unsigned int interval) {
    return Json::Value();
}

void commandanalysis::GeneratGetDeviceSN(int _session_id, Json::Value& _json_value) {
    _json_value[JSON_REQ_CMD] = JSON_REQ_CMD_GETSN;
}

std::string commandanalysis::IvsFormatToString(int ivs_format) {
    std::string sRet = vzsdk::IVSFormat_JSON;
    switch (ivs_format) {
    case vzsdk::FORMAT_BIN: {
        sRet = vzsdk::IVSFormat_BIN;
        break;
    }
    case vzsdk::FORMAT_JSON: {
        sRet = vzsdk::IVSFormat_JSON;
        break;
    }
    default:
        break;
    }
    return sRet;
}

void commandanalysis::GeneratIVSResult(bool _enable_result, int _format, bool _enable_img, int _img_type, Json::Value& req_json) {
    req_json[JSON_REQ_CMD]                    = JSON_PUSH_CMD_IVSRESULT;
    req_json[JSON_REQ_IVSRESULT_ENABLE]       = _enable_result;
    req_json[JSON_REQ_IVSRESULT_FORMAT]       = commandanalysis::IvsFormatToString(_format);
    req_json[JSON_REQ_IVSRESULT_ENABLE_IMAGE] = _enable_img;
    req_json[JSON_REQ_IVSRESULT_IMAGE_TYPE]   = _img_type;
}

void commandanalysis::GeneratForceTrigger(Json::Value& _json_value) {
    _json_value[JSON_REQ_CMD] = JSON_REQ_CMD_FORCETRIGGER;
}

Json::Value commandanalysis::GeneratGetGPIOValueCmd(int gpio) {

    Json::Value root;

    root["cmd"]		= "get_gpio_value";
    root["gpio"]	= gpio;

    return root;
}

Json::Value commandanalysis::GeneratSetGPIOAutoCmd(int gpio, int duration) {
    string cmd;

    Json::Value root;

    root["cmd"]		= "ioctl";
    root["io"]		= gpio;
    root["value"]	= 2;		//VALUE 0:关，1开，2先通后断
    root["delay"]	= duration;

    return root;
}

Json::Value commandanalysis::GeneratImportWlistVehicleCmd(const VZ_LPR_WLIST_VEHICLE *item) {
    // 起效的时间
    char enable_time[64] = {0};
    sprintf(enable_time, "%d-%02d-%02d %02d:%02d:%02d", item->struTMEnable.nYear, item->struTMEnable.nMonth, item->struTMEnable.nMDay,
            item->struTMEnable.nHour, item->struTMEnable.nMin, item->struTMEnable.nSec);

    // 到期时间
    char overdue_time[64] = {0};
    sprintf(overdue_time, "%d-%02d-%02d %02d:%02d:%02d", item->struTMOverdule.nYear, item->struTMOverdule.nMonth, item->struTMOverdule.nMDay,
            item->struTMOverdule.nHour, item->struTMOverdule.nMin, item->struTMOverdule.nSec);

    string cmd;

    Json::Value root;
    Json::Value record;

    root["cmd"]				= "white_list_operator";
    root["operator_type"]	= "update_or_add";

    record["index"]				= item->uVehicleID;
    record["plate"]				= item->strPlateID;
    record["customer_id"]		= item->uCustomerID;
    record["enable"]			= item->bEnable;
    record["enable_time"]		= enable_time;
    record["overdue_time"]		= overdue_time;
    record["time_seg_enable"]	= item->bUsingTimeSeg;
    record["need_alarm"]		= item->bAlarm;
    record["vehicle_code"]		= item->strCode;
    record["vehicle_comment"]	= item->strComment;

    root["dldb_rec"] = record;

    return root;
}

Json::Value commandanalysis::GeneratDeleteWlistVehicleCmd(const char* plate_code) {
    Json::Value root;

    root["cmd"]				= "white_list_operator";
    root["operator_type"]	= "delete";
    root["plate"]			= plate_code;

    return root;
}

Json::Value commandanalysis::GeneratQueryWlistVehicleCmd(const char* plate_code) {

    Json::Value root;

    root["cmd"]				= "white_list_operator";
    root["operator_type"]	= "select";
    root["sub_type"]		= "plate";
    root["plate"]			= plate_code;

    return root;
}

void commandanalysis::ParseTTransmissionResponse(Json::Value &root, TTRANSMISSION_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["subcmd"].isNull() ) {
        value->subcmd = root["subcmd"].asCString( );
    }

    if (!root["response"].isNull()) {
        value->response = root["response"].asCString( );
    }

    if ( !root["datalen"].isNull() ) {
        value->datalen = root["datalen"].asUInt( );
    }

    if (!root["comm"].isNull()) {
        value->comm = root["comm"].asUInt();
    }

    if (!root["data"].isNull()) {
        value->data = root["data"].asCString();
    }
}

void commandanalysis::ParseOfflineResponse(Json::Value &root, OFFLINE_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["subcmd"].isNull() ) {
        value->subcmd = root["subcmd"].asCString( );
    }

    if (!root["response"].isNull()) {
        value->response = root["response"].asCString( );
    }
}

void commandanalysis::ParseRecordResponse(Json::Value &root, RECORD_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["id"].isNull() ) {
        value->id = root["id"].asUInt( );
    }

    if (!root["size"].isNull()) {
        value->size = root["size"].asUInt( );
    }
}

void commandanalysis::ParseGPIOResponse(Json::Value &root, GPIO_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["gpio"].isNull() ) {
        value->gpio = root["gpio"].asUInt( );
    }

    if (!root["value"].isNull()) {
        value->val = root["value"].asUInt( );
    }
}

void commandanalysis::ParseMaxRecResponse(Json::Value &root, MAX_REC_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["max_id"].isNull() ) {
        value->max_id = root["max_id"].asUInt( );
    }
}

void commandanalysis::ParseTimestampResponse(Json::Value &root, TIMESTAMP_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["timestamp"].isNull() ) {
        value->timestamp = root["timestamp"].asUInt( );
    }
}

void commandanalysis::ParseWhiteListOperatorResponse(Json::Value &root, WHITE_LIST_OPERATOR_RESPONSE *value ) {
    if ( value == NULL ) {
        return;
    }

    if ( !root["operator_type"].isNull() ) {
        value->operator_type = root["operator_type"].asCString( );
    }

    if ( !root["state"].isNull() ) {
        value->state = root["state"].asCString( );
    }

    if ( !root["dldb_rec"].isNull() ) {
        value->record_count = root["dldb_rec"].size();
    }
}

void commandanalysis::ParseWhiteListRecordResponse(Json::Value &item, VZ_LPR_WLIST_VEHICLE &wlist ) {
    if ( item["index"].isInt() ) {
        wlist.uVehicleID = item["index"].asInt();
    }

    if ( item["plate"].isString() ) {
        strncpy( wlist.strPlateID, item["plate"].asCString(), VZ_LPR_WLIST_LP_MAX_LEN - 1 );
    }

    if ( item["ustomer_id"].isInt() ) {
        wlist.uCustomerID		= item["ustomer_id"].asInt();
    }

    if ( item["enable"].isInt() ) {
        wlist.bEnable			= item["enable"].asInt();
    }

    if ( item["enable_time"].isString() ) {
        const char *enable_time = item["enable_time"].asCString();
        sscanf(enable_time, "%d-%d-%d %d:%d:%d", &wlist.struTMEnable.nYear, &wlist.struTMEnable.nMonth,
               &wlist.struTMEnable.nMDay, &wlist.struTMEnable.nHour, &wlist.struTMEnable.nMin, &wlist.struTMEnable.nSec);

        wlist.bEnableTMEnable	= 1;
    }

    if ( item["overdue_time"].isString() ) {
        const char *overdue_time = item["overdue_time"].asCString();
        sscanf(overdue_time, "%d-%d-%d %d:%d:%d", &wlist.struTMOverdule.nYear, &wlist.struTMOverdule.nMonth,
               &wlist.struTMOverdule.nMDay, &wlist.struTMOverdule.nHour, &wlist.struTMOverdule.nMin, &wlist.struTMOverdule.nSec);

        wlist.bEnableTMOverdule = 1;
    }

    if ( item["time_seg_enable"].isInt() ) {
        wlist.bUsingTimeSeg		= item["time_seg_enable"].asInt();
    }

    if ( item["need_alarm"].isInt() ) {
        wlist.bAlarm			= item["need_alarm"].asInt();
    }

    if ( item["vehicle_code"].isString() ) {
        strncpy(wlist.strCode, item["vehicle_code"].asCString(), VZ_LPR_WLIST_VEHICLE_CODE_LEN - 1);
    }

    if( item["vehicle_comment"].isString() ) {
        strncpy(wlist.strComment, item["vehicle_comment"].asCString(), VZ_LPR_WLIST_VEHICLE_COMMENT_LEN - 1);
    }

}

void commandanalysis::ParsePlateResultResponse(Json::Value &root, TH_PlateResult &result, int &fullImgSize, int &clipImgSize) {
    Json::Value& plate = root["PlateResult"];

    if (plate["license"].isString()) {
        strcpy(result.license, plate["license"].asCString());
    }

    if (plate["colorValue"].isString()) {
        strcpy(result.color, plate["colorValue"].asCString());
    }

    if (plate["colorType"].isInt()) {
        result.nColor = plate["colorType"].asInt();
    }

    if (plate["type"].isInt()) {
        result.nType = plate["type"].asInt();
    }

    if (plate["confidence"].isInt()) {
        result.nConfidence = plate["confidence"].asInt();
    }

    if (plate["bright"].isInt()) {
        result.nBright = plate["bright"].asInt();
    }

    if (plate["direction"].isInt()) {
        result.nDirection = plate["direction"].asInt();
    }

    // 车牌位置
    if (plate["location"]["RECT"]["left"].isInt()) {
        result.rcLocation.left = plate["location"]["RECT"]["left"].asInt();
    }

    if (plate["location"]["RECT"]["top"].isInt()) {
        result.rcLocation.top = plate["location"]["RECT"]["top"].asInt();
    }

    if (plate["location"]["RECT"]["right"].isInt()) {
        result.rcLocation.right = plate["location"]["RECT"]["right"].asInt();
    }

    if (plate["location"]["RECT"]["bottom"].isInt()) {
        result.rcLocation.bottom = plate["location"]["RECT"]["bottom"].asInt();
    }

    if (plate["timeUsed"].isInt()) {
        result.nTime = plate["timeUsed"].asInt();
    }

    if (plate["carBright"].isInt()) {
        result.nCarBright = plate["carBright"].asInt();
    }

    if (plate["carColor"].isInt()) {
        result.nCarColor = plate["carColor"].asInt();
    }

    if (plate["timeStamp"]["Timeval"]["sec"].isInt()) {
        result.tvPTS.uTVSec = plate["timeStamp"]["Timeval"]["sec"].asInt();
    }

    if (plate["timeStamp"]["Timeval"]["usec"].isInt()) {
        result.tvPTS.uTVUSec = plate["timeStamp"]["Timeval"]["usec"].asInt();
    }

    if (plate["triggerType"].isInt()) {
        result.uBitsTrigType = plate["triggerType"].asInt();
    }

    if (root["id"].isInt()) {
        result.uId = root["id"].asInt();
    }

    if (root["timeString"].isString()) {
        int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;
        const char *timeSting = root["timeString"].asCString();
        sscanf(timeSting, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);

        result.struBDTime.bdt_year = nYear;
        result.struBDTime.bdt_mon = nMonth;
        result.struBDTime.bdt_mday = nDay;
        result.struBDTime.bdt_hour = nHour;
        result.struBDTime.bdt_min = nMin;
        result.struBDTime.bdt_sec = nSec;

    }

    if (root["fullImgSize"].isInt()) {
        fullImgSize = root["fullImgSize"].asInt();
    }

    if (root["clipImgSize"].isInt()) {
        clipImgSize = root["clipImgSize"].asInt();
    }
}
