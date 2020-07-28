//
// Created by Wei00161@umn.edu on 2020/5/14.
//

#ifndef LEVELDB_DM_ZONE_H
#define LEVELDB_DM_ZONE_H

#include "zone_namespace.h"
#include <fstream>
using namespace std;


namespace leveldb {
    enum ZoneHotness {HOT, COLD};
    static const std::string ZoneHotness[] = { "HOT", "COLD"};

    class DmZone : public Zone {
    public:
        DmZone(fstream& fs){};
        ~DmZone(){};
        //need to assign: ZoneID, write_pointer, ....
        DmZone(fstream& fs,size_t id);

        //based on Zone id to open file
        //set C++ put pointer according to the write_pointer in zone
        Status OpenZone() override;

        Status CloseZone() override;

        Status FinishZone() override;

        ZoneInfo ReportZone() override;
        //equal to erase ???
        Status ResetWritePointer() override;

        Status ZoneRead(ZoneAddress addr,  char* data) override;

        Status ZoneWrite(ZoneAddress addr, const char* data) override;

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
        //fstream& modify_zone_;
        size_t erase_count_ = 0;
        enum ZoneHotness zone_hotness_ = COLD;
    };


    class DmZoneNamespace : public ZoneNamespace {
    public:
        DmZoneNamespace();
        ~DmZoneNamespace()= default;
        static shared_ptr<DmZoneNamespace> CreatZoneNamespace(){
            return shared_ptr<DmZoneNamespace>(new DmZoneNamespace());
        }
        size_t GetZoneCount() override{
            return zones_.size();
        }

        // Create a new zone.
        Status NewZone() override;

        shared_ptr<Zone> GetZone(int id) override {
            Status status;
            shared_ptr<Zone> res_zone;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                res_zone = it->second;
                //status = Status::OK();
            } else {
                status = Status::NotFound("[Zone_namespace.h] [RemoveZone] no zone to get, zone id: "+ to_string(id));
            }
            return res_zone;
        }

        Status RemoveZone(int id) override{
            Status status;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                zones_.erase(it);
                status = Status::OK();
            } else {
                status = Status::NotFound("[Zone_namespace.h] [RemoveZone] no zone to remove, zone id: "+ to_string(id));
            }
            return status;
        }

        Status InitZNS(const char* dir_name) override;
        Status InitZone(const char *path, const char *filename,  char *filepath) override;
        //Status Write(ZoneAddress addr, const char* data) override;
        //Status Read(ZoneAddress addr,  char* data) override;
        Status MigrateData(DmZone& target, DmZone& source);
        Status GC();
    private:
        int next_zone_id_ = 0;
        // the zone_id in zones_ is PBA
        std::map<int, shared_ptr<DmZone>> zones_;
        // need a mapping between LBA to PBA
        // first int is Logical zone id while second int is physical zone id
        // this mapping only used in DM & HA ZNS, for HM ZNS, all GC and migration will be done by host
        std::map<int, int> LBA_2_PBA_;
    };
}
#endif //LEVELDB_DM_ZONE_H
