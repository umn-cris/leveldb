#ifndef ZONE_NAMESPACEH_
#define ZONE_NAMESPACEH_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <list>
#include "leveldb/status.h"

namespace leveldb {

enum ZoneType {CONVENTIONAL, SEQUENTIAL_WRITE_PREFERRED, SEQUENTIAL_WRITE_REQUIRED};
enum ZoneCondition {OPEN, CLOSED};
enum ZoneState {SEQUENTIAL, NON_SEQUENTIAL};

struct ZoneInfo {
    int id;
    size_t first_LBA = -1;
    size_t size;
    size_t write_pointer;
    ZoneType zone_type;
    ZoneCondition zone_condition;
    ZoneState zone_state;
};

class Zone {
public:
    Zone();
    virtual ~Zone()= default;

    // lookup file to open
    // if not exist, create
    // file name is "Zone" + id
    virtual Status OpenZone();

    virtual Status CloseZone();

    virtual Status FinishZone();

    virtual ZoneInfo ReportZone();

    virtual Status ResetWritePointer();

    virtual Status Read();

    virtual Status Write();

protected:

    ZoneInfo zone_info_; 
};


struct ZoneAddress {
    int zone_id;
    size_t offset;
};


class ZoneNamespace {
public:
    ZoneNamespace();
    virtual ~ZoneNamespace()= default;

    // Get the number of zones.
    size_t GetZoneCount(){
       return zones_.size();
    };

    // Create a new zone.
    virtual Status NewZone() = 0;

    // Remove an existing zone based on its id.
    Status RemoveZone(int id){
        Status status;
        auto it = zones_.find(id);
        if(it!=zones_.end()){
            zones_.erase(it);
            status = Status::OK();
        } else{
            status = Status::NotFound("Zone NotFound status message");
            std::cout<<"[Zone_namespace.h] [RemoveZone] no zone "<<id<<"to remove"<<std::endl;
        }
        return status;
    }

    Status GetZone(int id, Zone& res_zone){
        Status status;
        auto it = zones_.find(id);
        if(it!=zones_.end()){
            res_zone = it->second;
            status = Status::OK();
        } else{
            status = Status::NotFound("Zone NotFound status message");
            std::cout<<"[Zone_namespace.h] [RemoveZone] no zone "<<id<<"to get"<<std::endl;
        }
        return status;
    }

protected:
    int next_zone_id_ = 0;
    std::map<int, Zone> zones_;
};

}

#endif // ZONE_NAMESPACEH_