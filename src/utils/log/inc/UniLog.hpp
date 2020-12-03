#ifndef UTILS_LOG_INC_UNI_LOG_HPP_
#define UTILS_LOG_INC_UNI_LOG_HPP_

#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/LoggingConfigurator.h"

using namespace std;

#define LOG_CONFIG_PATH "./config/log_conf.properties"
#define LOGT1(logger, s) poco_information(logger, s)
#define LOGE1(logger, s) poco_error(logger, s)

class UniLog {
private:
    static UniLog mLog;
private:
    UniLog() {
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConf = new Poco::Util::PropertyFileConfiguration(LOG_CONFIG_PATH);
        Poco::Util::LoggingConfigurator log_configurator;
        log_configurator.configure(pConf);
    }
public:
    static Poco::Logger& getLogger(const string& tag) {
        return Poco::Logger::get(tag.c_str());
    }
};

#endif  //  UTILS_LOG_INC_UNI_LOG_HPP_