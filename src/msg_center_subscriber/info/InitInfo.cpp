#include "InitInfo.hpp"
#include "uni_log.h"

CJsonObject InitInfo :: mInfo;
pthread_mutex_t InitInfo :: mMutex = PTHREAD_MUTEX_INITIALIZER;

#define INIT_INFO_TAG "init_info"

int InitInfo :: getInfo(CJsonObject&jinfo) {
    pthread_mutex_lock(&mMutex);
    if (false == mInfo.IsEmpty()) {
        jinfo = mInfo;
        pthread_mutex_unlock(&mMutex);
        return 0;
    } 
    pthread_mutex_unlock(&mMutex);
    return -1;
}

int InitInfo :: getBuilding(int building_id, CJsonObject&jbuilding) {
    int i;
    int id = 0;
    pthread_mutex_lock(&mMutex);
    if (false == mInfo.IsEmpty()) {
        for (i = 0; i < mInfo["buildings"].GetArraySize(); i++) {
            mInfo["buildings"][i].Get("id", id);
            if (id == building_id) {
                jbuilding = mInfo["buildings"][i];
                pthread_mutex_unlock(&mMutex);
                LOGE(INIT_INFO_TAG, "building found for id %d", building_id);
                return 0;
            }
        }
    }
    pthread_mutex_unlock(&mMutex);
    LOGE(INIT_INFO_TAG, "building not found for id %d", building_id);
    return -1;
}

int InitInfo :: getUnit(int unit_id, CJsonObject&junit) {
    int i, j;
    int id = 0;
    CJsonObject jbuilding;
    pthread_mutex_lock(&mMutex);
    if (false == mInfo.IsEmpty()) {
        for (i = 0; i < mInfo["buildings"].GetArraySize(); i++) {
            jbuilding = mInfo["buildings"][i];
            for (j = 0; j < jbuilding["units"].GetArraySize(); j++) {
                jbuilding["units"][j].Get("id", id);
                if (id == unit_id) {
                    junit = jbuilding["units"][j];
                    pthread_mutex_unlock(&mMutex);
                    LOGT(INIT_INFO_TAG, "unit found for id %d", unit_id);
                    return 0;
                }
            }
        }
    } 
    pthread_mutex_unlock(&mMutex);
    LOGE(INIT_INFO_TAG, "unit not found for id %d", unit_id);
    return -1;
}

int InitInfo :: getUnit(int unit_id, CJsonObject& jbuilding, CJsonObject& junit) {
    int j;
    int id = 0;
    pthread_mutex_lock(&mMutex);
    if (false == jbuilding.IsEmpty()) {
        for (j = 0; j < jbuilding["units"].GetArraySize(); j++)
        {
            jbuilding["units"][j].Get("id", id);
            if (id == unit_id)
            {
                junit = jbuilding["units"][j];
                pthread_mutex_unlock(&mMutex);
                LOGT(INIT_INFO_TAG, "unit found for id %d", unit_id);
                return 0;
            }
        }
    } 
    pthread_mutex_unlock(&mMutex);
    LOGE(INIT_INFO_TAG, "unit not found for id %d", unit_id);
    return -1;
}

int InitInfo :: getHome(int home_id, CJsonObject&jhome) {
    int i, j, k;
    int id = 0;
    CJsonObject jbuilding;
    CJsonObject junit;
    pthread_mutex_lock(&mMutex);
    if (false == mInfo.IsEmpty()) {
        for (i = 0; i < mInfo["buildings"].GetArraySize(); i++) {
            jbuilding = mInfo["buildings"][i];
            for (j = 0; j < jbuilding["units"].GetArraySize(); j++) {
                junit = jbuilding["units"][j];
                for (k = 0; k < junit["homes"].GetArraySize(); k++) {
                    junit["homes"][k].Get("id", id);
                    if (id == home_id) {
                        jhome = junit["homes"][k];
                        pthread_mutex_unlock(&mMutex);
                        LOGT(INIT_INFO_TAG, "home found for id %d: %s", home_id, jhome.ToString().c_str());
                        return 0;
                    }
                }
            }
        }
    }
    LOGE(INIT_INFO_TAG, "home not found for id %d", home_id);
    pthread_mutex_unlock(&mMutex);
    return -1;
}

int InitInfo :: getHome(int home_id, CJsonObject& jbuilding, CJsonObject&jhome) {
    int j, k;
    int id = 0;
    CJsonObject junit;
    pthread_mutex_lock(&mMutex);
    if (false == jbuilding.IsEmpty()) {
        for (j = 0; j < jbuilding["units"].GetArraySize(); j++)
        {
            junit = jbuilding["units"][j];
            for (k = 0; k < junit["homes"].GetArraySize(); k++)
            {
                junit["homes"][k].Get("id", id);
                if (id == home_id)
                {
                    jhome = junit["homes"][k];
                    pthread_mutex_unlock(&mMutex);
                    LOGT(INIT_INFO_TAG, "home found for id %d: %s", home_id, jhome.ToString().c_str());
                    return 0;
                }
            }
        }
    }
    LOGE(INIT_INFO_TAG, "home not found for id %d", home_id);
    pthread_mutex_unlock(&mMutex);
    return -1;
}

int InitInfo :: setInfo(const CJsonObject& info) {
    pthread_mutex_lock(&mMutex);
    mInfo = info;
    pthread_mutex_unlock(&mMutex);
    return 0;
}