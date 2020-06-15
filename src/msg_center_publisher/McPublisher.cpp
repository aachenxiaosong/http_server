#include "McPublisher.hpp"
#include "McBroker.hpp"
#include "uni_uuid.h"
#include "uni_device.h"
#include "uni_log.h"

#define MC_PUBLISHER "mc_publisher"

McPublisher :: McPublisher(const char* name) {
    this->name = name;
    LOGT(MC_PUBLISHER, "McPublisher %s is created", name);
}
