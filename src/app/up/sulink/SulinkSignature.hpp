#ifndef APP_UP_SULINK_SULINK_SIGNATURE_HPP_
#define APP_UP_SULINK_SULINK_SIGNATURE_HPP_

#include "Md5Sum.hpp"
#include "UrlCodec.hpp"
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class SulinkSignature {
public:
    static string create(const map<string, string>& params, const string& access_key,
                      const string& secret_key, const string& timestamp) {
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
        new_params["AccessKey"] = access_key;
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
        param_str.append("SecretKey=" + secret_key);
        //5, md5(capital)
        md5 =  Md5Sum::md5(param_str);
        transform(md5.begin(), md5.end(), md5.begin(), ::toupper);
        
        return md5;
    }
};

#endif  //  APP_UP_SULINK_SULINK_SIGNATURE_HPP_