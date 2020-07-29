#include "WlongInitMsgHandler.hpp"
#include "InitInfo.hpp"
#include "WlongHttpService.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "uni_log.h"
#include "configurable_info.h"

#define WLONG_INIT_MSG_TAG "wlong_init_msg"

WlongInitMsgHandler ::WlongInitMsgHandler() : IMcMsgHandler("wlong_init_msg")
{
    string init_info;
    mHttpService = NULL;
    LOGT(WLONG_INIT_MSG_TAG, "WlongInitMsgHandler is created");
    _import_init_info(init_info);
    if (init_info.size() > 0) {
        handle(init_info);
    }
}

int WlongInitMsgHandler ::handle(string &msg)
{
    int ret;
    string msg_type;
    CJsonObject djson;
    CJsonObject sjson(msg);
    string work_mode = "";
    if (true != sjson.Get("msgType", msg_type) || msg_type.compare("LIFT_CTRL_CMD_INIT") != 0) {
        return -1;
    }
    if (true != sjson.Get("workMode", work_mode)  || 0 != work_mode.compare("1")) {
        LOGT(WLONG_INIT_MSG_TAG, "work mode %s is not for wlong", work_mode.c_str());
        return -1;
    }
    LOGT(WLONG_INIT_MSG_TAG, "msg %s is handled by wlong init msg handler", msg_type.c_str());
    ret = _parse(djson, sjson);
    if (ret == 0) {
        InitInfo :: setInfo(djson);
        std::cout << djson.ToString() <<std::endl;
        if (mHttpService == NULL) {
            mHttpService = new WlongHttpService();
        }
        mHttpService->unRegisterHttpHandler();
        mHttpService->registerHttpHandler();
        _save_init_info(msg);
        LOGT(WLONG_INIT_MSG_TAG, "work mode 1 is for wlong, http service registered");
    } 
    return 0;
}

int WlongInitMsgHandler :: _import_init_info(string &info) {
    LOGT(WLONG_INIT_MSG_TAG, "importing init info ... ");
    ifstream ifs(INIT_INFO_CONFIG_FILE);
    ostringstream oss;
    oss << ifs.rdbuf();
    info = oss.str();
    ifs.close();
    return 0;
}

int WlongInitMsgHandler :: _save_init_info(string &info) {
    LOGT(WLONG_INIT_MSG_TAG, "saving init info ... ");
    ofstream ofs(INIT_INFO_CONFIG_FILE);
    ofs << info;
    ofs.close();
    return 0;
}

int WlongInitMsgHandler :: _parse_home(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    if (true != sjson["extension"].Get("homeNo", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson["extension"].Get("floorLocation", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("floor", svalue);
    if (true != sjson["extension"].Get("clusterControllerId", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("liftHall", svalue);
    return 0;
L_ERROR:
    LOGE(WLONG_INIT_MSG_TAG, "parse home %d failed", id);
    return -1;
}

int WlongInitMsgHandler :: _parse_unit(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    int j = 0;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    if (true != sjson["extension"].Get("unitNo", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson["extension"].Get("floorMap", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("floorMap", CJsonObject(svalue));
    if (true != sjson.Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
        return 0;
    }
    djson.AddEmptySubArray("homes");
    j = 0;
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        if (jvalue[i].Get("typeCode", svalue) == false || svalue.compare("house_home") != 0) {
            LOGT(WLONG_INIT_MSG_TAG, "skip type code %s house_home list", svalue.c_str());
            continue;
        }
        djson["homes"].Add(CJsonObject("{}"));
        if (0 != _parse_home(djson["homes"][j], jvalue[i]))
        {
            LOGE(WLONG_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
        j++;
    }
    return 0;
L_ERROR:
    LOGE(WLONG_INIT_MSG_TAG, "parse unit %d failed", id);
    return -1;
}

int WlongInitMsgHandler :: _parse_building(CJsonObject &djson, CJsonObject &sjson)
{
    int id = 0;
    int ivalue;
    string svalue;
    CJsonObject jvalue;
    int j = 0;
    if (true != sjson.Get("id", ivalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    id = ivalue;
    djson.Add("id", ivalue);
    if (true != sjson.Get("extension", jvalue) ||
        true != sjson["extension"].Get("buildingNo", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("number", svalue);
    if (true != sjson.Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.AddEmptySubArray("units");
    j = 0;
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        if (jvalue[i].Get("typeCode", svalue) == false || svalue.compare("unit") != 0) {
            LOGT(WLONG_INIT_MSG_TAG, "skip type code %s unit list", svalue.c_str());
            continue;
        }
        djson["units"].Add(CJsonObject("{}"));
        if (0 != _parse_unit(djson["units"][j], jvalue[i]))
        {
            LOGE(WLONG_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
        j++;
    }
    return 0;
L_ERROR:
    LOGE(WLONG_INIT_MSG_TAG, "parse building %d failed", id);
    return -1;
}

int WlongInitMsgHandler :: _parse(CJsonObject &djson, CJsonObject &sjson)
{
    string svalue;
    CJsonObject jvalue;
    int j = 0;
    if (true != sjson.Get("reqId", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("reqId", svalue);
    if (true != sjson.Get("workMode", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("workMode", svalue);
    if (true != sjson.Get("entityCode", svalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("entityCode", svalue);

    if (true != sjson.Get("extendData", jvalue))
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.Add("wlong", jvalue);

    if (true != sjson.Get("spaceFramework", jvalue) ||
        true != sjson["spaceFramework"].Get("spaceEntityList", jvalue) ||
        false == jvalue.IsArray())
    {
        LOGE(WLONG_INIT_MSG_TAG, "parse failed");
        goto L_ERROR;
    }
    djson.AddEmptySubArray("buildings");
    j = 0;
    for (int i = 0; i < jvalue.GetArraySize(); i++)
    {
        if (jvalue[i].Get("typeCode", svalue) == false || svalue.compare("building") != 0) {
            LOGT(WLONG_INIT_MSG_TAG, "skip type code %s building list", svalue.c_str());
            continue;
        }
        djson["buildings"].Add(CJsonObject("{}"));
        if (0 != _parse_building(djson["buildings"][j], jvalue[i]))
        {
            LOGE(WLONG_INIT_MSG_TAG, "parse failed");
            goto L_ERROR;
        }
        j++;
    }
    return 0;
L_ERROR:
    LOGE(WLONG_INIT_MSG_TAG, "parse failed");
    return -1;
}