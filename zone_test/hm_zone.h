//
// Created by Wei00161@umn.edu on 2020/5/15.
//

#ifndef LEVELDB_HM_ZONE_H
#define LEVELDB_HM_ZONE_H
#include "zone_namespace.h"
#include <fstream>
//TODO write pointer position look up
using namespace std;
namespace leveldb {

    class HmZone : public Zone {
    public:
        HmZone(fstream& fs):modify_zone_(fs){};
        ~HmZone(){};
        //need to assign: ZoneID, write_pointer, ....
        HmZone(fstream& fs,size_t id);

        //based on Zone id to open file
        //set C++ put pointer according to the write_pointer in zone
        Status OpenZone();

        Status CloseZone();

        Status FinishZone();

        ZoneInfo ReportZone();

        Status ResetWritePointer();

        Status Read(ZoneAddress addr, const char* data) {Status s; return s;}

        Status Write(ZoneAddress addr, char* data) {Status s; return s;}

        string ToString() const {

            string str = to_string(zoneInfo_.id);

            str+=";";
            str+=to_string(zoneInfo_.write_pointer);

            str+=";";
            str+=TypeStr[zoneInfo_.zone_type];

            str+=";";
            str+=ConditionStr[zoneInfo_.zone_condition];

            str+=";";
            str+=StateStr[zoneInfo_.zone_state];
            return str;
        }

    private:
        fstream& modify_zone_;
    };


    class HmZoneNamespace : public ZoneNamespace {
    public:
        HmZoneNamespace();
        ~HmZoneNamespace()= default;

        size_t GetZoneCount() {
            return zones_.size();
        }

        // Create a new zone.
        Status NewZone();

        Status GetZone(int id, Zone &res_zone) {
            Status status;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                res_zone = it->second;
                status = Status::OK();
            } else {
                status = Status::NotFound("Zone NotFound status message");
                std::cout << "[Zone_namespace.h] [RemoveZone] no zone " << id << "to get" << std::endl;
            }
            return status;
        }

        Status RemoveZone(int id) {
            Status status;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                zones_.erase(it);
                status = Status::OK();
            } else {
                status = Status::NotFound("Zone NotFound status message");
                std::cout << "[Zone_namespace.h] [RemoveZone] no zone " << id << "to remove" << std::endl;
            }
            return status;
        }

        Status InitZNS(const char* dir_name);
        Status InitZone(const char *path, const char *filename,  char *filepath);
    private:
        int next_zone_id_ = 0;
        std::map<int, HmZone> zones_;
    };
}
#endif //LEVELDB_HM_ZONE_H
