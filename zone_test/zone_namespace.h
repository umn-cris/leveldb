#ifndef ZONE_NAMESPACEH_
#define ZONE_NAMESPACEH_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <list>
#include "leveldb/status.h"
static const size_t ZONESIZE = 512*1024*1024;
namespace leveldb {

enum ZoneType {CONVENTIONAL, SEQUENTIAL_WRITE_PREFERRED, SEQUENTIAL_WRITE_REQUIRED};
static const std::string TypeStr[] = { "CONVENTIONAL", "SEQUENTIAL_WRITE_PREFERRED", "SEQUENTIAL_WRITE_REQUIRED" };
enum ZoneCondition {OPEN, CLOSED};
static const std::string ConditionStr[] = { "OPEN", "CLOSED"};
enum ZoneState {SEQUENTIAL, NON_SEQUENTIAL};
static const std::string StateStr[] = { "SEQUENTIAL", "NON_SEQUENTIAL"};

    struct ZoneInfo {
        int id = 0;
        size_t first_LBA = 0;
        size_t size = 0;
        size_t write_pointer = 0;
        ZoneType zone_type;
        ZoneCondition zone_condition;
        ZoneState zone_state;
    };

class Zone {
public:
    Zone()= default;
    virtual ~Zone()= default;



    // lookup file to open
    // if not exist, create
    // file name is "Zone" + id
    virtual Status OpenZone()=0;

    virtual Status CloseZone()=0;

    virtual Status FinishZone()=0;

    virtual ZoneInfo ReportZone()=0;

    virtual Status ResetWritePointer()=0;

    virtual Status Read()=0;

    virtual Status Write()=0;

protected:
    ZoneInfo zoneInfo_;
    //ZoneInfo* zone_info_;
};


struct ZoneAddress {
    int zone_id;
    size_t offset;
};


class ZoneNamespace {
public:
    ZoneNamespace()= default;
    virtual ~ZoneNamespace()= default;

    // Get the number of zones.
    virtual size_t GetZoneCount() = 0;

    // Create a new zone.
    virtual Status NewZone() = 0;

    // Remove an existing zone based on its id.
    virtual Status RemoveZone(int id) = 0;

    virtual Status GetZone(int id, Zone& res_zone) = 0;

    virtual Status InitZNS(const char* dir_name) = 0;

    virtual Status InitZone(const char *path, const char *filename,  char *filepath) = 0;

};

}

#endif // ZONE_NAMESPACEH_