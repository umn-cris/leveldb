#ifndef ENV_ZNS_H_
#define ENV_ZNS_H_

#include <map>

#include "leveldb/env.h"
#include "zone_test/zone_namespace.h"

namespace leveldb {

enum ZoneWritePattern {SINGLE_LOG, LEVELED_LOG, WEAR_LEVELING_AWARE};

/*
class Env_Zns : public leveldb::Env {
public:
    Env_Zns(ZoneNamespace &zns);
    ~Env_Zns();

private:
    // uint32_t zone_total_count_;
    // uint32_t zone_size_;

    ZoneWritePattern zone_write_pattern_ = SINGLE_LOG;
    std::map<std::string, ZoneAddress> filename2zone_address_;

    // For the baseline design: single log
    uint32_t next_zone_id_;
    uint32_t current_write_pointer_ = 0;
    std::set<ZoneAddress> free_zone_spaces_;
};
*/

}

#endif // ENV_ZNS_H_
