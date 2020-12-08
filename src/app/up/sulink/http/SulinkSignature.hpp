#ifndef APP_UP_SULINK_HTTP_SULINK_SIGNATURE_HPP_
#define APP_UP_SULINK_HTTP_SULINK_SIGNATURE_HPP_

#include "configurable_info.h"
#include "UniUtil.hpp"
#include "UrlCodec.hpp"
#include "SulinkConfigData.hpp"
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class SulinkSignature {
public:
    static string build(const map<string, string>& params, const string& timestamp) {
        //1, remove empty param
        map<string, string> new_params;
        map<string, string> :: const_iterator cit;
        for (cit = params.begin(); cit != params.end(); cit++) {
            if (cit->second.empty() == true) {
                continue;
            }
            new_params.insert(pair<string, string>(cit->first, cit->second));
        }
        //2, add accesskey, timestamp, sort in ascending order(map does this by default)
        new_params["AccessKey"] = SulinkConfigData::getAccessKey();
        new_params["Timestamp"] = timestamp;
        //3, transform to string, format: key1=value1&key2=value2
        string param_str;
        string md5;
        map<string, string> :: iterator it;
        for (it = new_params.begin(); it != new_params.end(); it++) {
            param_str.append(it->first + "=" + UrlCodec::encode(it->second) + "&");
            //param_str.append(it->second + "&");
        }
        //4, add secret_key at last
        param_str.append("SecretKey=" + string(SulinkConfigData::getSecretKey()));
        //5, md5(capital)
        md5 =  unisound::UniUtil::md5(param_str);
        transform(md5.begin(), md5.end(), md5.begin(), ::toupper);
        
        return md5;
    }
};

#endif  //  APP_UP_SULINK_HTTP_SULINK_SIGNATURE_HPP_
