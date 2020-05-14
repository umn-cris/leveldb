#ifndef ZONE_NAMESPACEH_
#define ZONE_NAMESPACEH_

#include <vector>
#include <map>
#include <string>
#include "leveldb/status.h"

namespace leveldb {

enum ZoneType {CONVENTIONAL, SEQUENTIAL_WRITE_PREFERRED, SEQUENTIAL_WRITE_REQUIRED};
enum ZoneCondition {OPEN, CLOSED};
enum ZoneState {SEQUENTIAL, NON_SEQUENTIAL};

struct ZoneInfo {
    int id;
    int first_LBA;
    int size;
    int write_pointer;
    ZoneType zone_type;
    ZoneCondition zone_condition;
    ZoneState zone_state;
};

class Zone {
public:
    Zone() = delete;
    virtual ~Zone();

    virtual Status OpenZone() = 0;

    virtual Status CloseZone() = 0;

    virtual Status FinishZone() = 0;

    virtual ZoneInfo ReportZone() = 0;

    virtual Status ResetWritePointer() = 0;

    virtual Status Read() = 0;

    virtual Status Write() = 0;

private:

    ZoneInfo zone_info_; 
};


struct ZoneAddress {
    int zone_id;
    size_t offset;
};


class ZoneNamespace {
public:
    ZoneNamespace() = delete;
    virtual ~ZoneNamespace();

    // Get the number of zones.
    virtual size_t GetZoneCount() = 0;

    // Create a new zone.
    virtual Status NewZone() = 0;

    // Remove an existing zone based on its id.
    virtual Status RemoveZone(int id) = 0;

private:
    size_t next_zone_id_ = 0;
    std::vector<Zone> zones_;
    std::map<int, std::vector<Zone>::iterator> zone_id2zone_;
};

}

#endif // ZONE_NAMESPACEH_