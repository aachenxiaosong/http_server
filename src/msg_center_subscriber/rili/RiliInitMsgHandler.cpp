#include "RiliInitMsgHandler.hpp"
#include "RiliInfo.hpp"
#include "RiliHttpService.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "uni_log.h"
#include "configurable_info.h"

#define RILI_INIT_MSG_TAG "rili_init_msg"

RiliInitMsgHandler ::RiliInitMsgHandler() : IMcMsgHandler("rili_init_msg")
{
    string init_info;
    mHttpService = NULL;
    LOGT(RILI_INIT_MSG_TAG, "RiliInitMsgHandler is created");
    _import_init_info(init_info);
    if (init_info.size() > 0) {
        handle(init_info);
    }
}

int RiliInitMsgHandler ::handle(string &msg)
{
    int ret;
    string msg_type;
    CJsonObject djson;
    CJsonObject sjson(msg);
    string work_mode = "";
    if (true != sjson.Get("msgType", msg_type) || msg_type.compare("LIFT_CTRL_CMD_INIT") != 0) {
        return -1;
    }
    if (true != sjson.Get("workMode", work_mode)  || 0 != work_mode.compare("2")) {
        LOGT(RILI_INIT_MSG_TAG, "work mode %s is not for rili", work_mode.c_str());
        return -1;
    }
    LOGT(RILI_INIT_MSG_TAG, "msg %s is handled by rili book init msg handler", msg_type.c_str());
    ret = _parse(djson, sjson);
    if (ret == 0) {
        RiliInfo :: setInfo(djson);
        std::cout << djson.ToString() <<std::endl;
        if (mHttpService == NULL) {
            mHttpService = new RiliHttpService();
        }
        mHttpService->unRegisterHttpHandler();
        mHttpService->registerHttpHandler();
        LOGT(RILI_INIT_MSG_TAG, "work mode 1 is for rili, http service registered");
    } 
    _save_init_info(msg);
    return 0;
}

int RiliInitMsgHandler :: _import_init_info(string &info) {
    LOGT(RILI_INIT_MSG_TAG, "importing init info ... ");
    ifstream ifs(INIT_INFO_CONFIG_FILE);
    ostringstream oss;
    oss << ifs.rdbuf();
    info = oss.str();
    ifs.close();
    return 0;
}

int RiliInitMsgHandler :: _save_init_info(string &info) {
    LOGT(RILI_INIT_MSG_TAG, "saving init info ... ");
    ofstream ofs(INIT_INFO_CONFIG_FILE);
    ofs << info;
    ofs.close();
    return 0;
}

int RiliInitMsgHandler :: _parse_home(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    if (true != sjson["extension"].Get("homeNo", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson["extension"].Get("floorLocation", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("floor", svalue);
    if (true != sjson["extension"].Get("clusterControllerId", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("liftHall", svalue);
    return 0;
L_ERROR:
    LOGE(RILI_INIT_MSG_TAG, "parse home %d failed", id);
    return -1;
}

int RiliInitMsgHandler :: _parse_unit(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    if (true != sjson["extension"].Get("unitNo", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson["extension"].Get("floorMap", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("floorMap", CJsonObject(svalue));
    if (true != sjson.Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
        return 0;
    }
    djson.AddEmptySubArray("homes");
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        djson["homes"].Add(CJsonObject("{}"));
        if (0 != _parse_home(djson["homes"][i], jvalue[i]))
        {
            LOGE(RILI_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
    }
    return 0;
L_ERROR:
    LOGE(RILI_INIT_MSG_TAG, "parse unit %d failed", id);
    return -1;
}

int RiliInitMsgHandler :: _parse_building(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue) ||
        true != sjson["extension"].Get("buildingNo", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson.Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.AddEmptySubArray("units");
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        djson["units"].Add(CJsonObject("{}"));
        if (0 != _parse_unit(djson["units"][i], jvalue[i]))
        {
            LOGE(RILI_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
    }
    return 0;
L_ERROR:
    LOGE(RILI_INIT_MSG_TAG, "parse building %d failed", id);
    return -1;
}

int RiliInitMsgHandler :: _parse(CJsonObject &djson, CJsonObject &sjson)
{
    string svalue;
    CJsonObject jvalue;
    if (true != sjson.Get("reqId", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("reqId", svalue);
    if (true != sjson.Get("workMode", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("workMode", svalue);
    if (true != sjson.Get("entityCode", svalue))
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("entityCode", svalue);

    // if (true != sjson.Get("extendData", jvalue))
    // {
    //     LOGE(RILI_INIT_MSG_TAG, "parse failed");
    //     goto L_ERROR;
    // }
    // djson.Add("rili", jvalue);

    if (true != sjson.Get("spaceFramework", jvalue) ||
        true != sjson["spaceFramework"].Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(RILI_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.AddEmptySubArray("buildings");
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        djson["buildings"].Add(CJsonObject("{}"));
        if (0 != _parse_building(djson["buildings"][i], jvalue[i]))
        {
            LOGE(RILI_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
    }
    return 0;
L_ERROR:
    LOGE(RILI_INIT_MSG_TAG, "parse failed");
    return -1;
}