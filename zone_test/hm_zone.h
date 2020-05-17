//
// Created by Wei00161@umn.edu on 2020/5/15.
//

#ifndef LEVELDB_HM_ZONE_H
#define LEVELDB_HM_ZONE_H
#include "zone_namespace.h"
//TODO write pointer position look up
namespace leveldb {

    class HmZone : public Zone {
    public:
        HmZone() = default;
        ~HmZone(){};
        explicit HmZone(size_t id){
            //zone_info_ = (struct ZoneInfo*)malloc(sizeof(struct ZoneInfo));
            zoneInfo_.id = id;
            zoneInfo_.size = 512;
        }


        Status OpenZone() {

        }

        Status CloseZone(){}

        Status FinishZone(){}

        ZoneInfo ReportZone(){}

        Status ResetWritePointer(){}

        Status Read(){}

        Status Write(){}

    };


    class HmZoneNamespace : public ZoneNamespace {
    public:
        HmZoneNamespace();
        ~HmZoneNamespace()= default;

        size_t GetZoneCount() {
            return zones_.size();
        }

        // Create a new zone.
        Status NewZone() {
            Status status;
            HmZone hmZone(next_zone_id_);
            auto it = zones_.emplace(next_zone_id_, hmZone);
            ++next_zone_id_;
            return status;
        }

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

    private:
        int next_zone_id_ = 0;
        std::map<int, HmZone> zones_;
    };
}
#endif //LEVELDB_HM_ZONE_H
