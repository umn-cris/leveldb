//
// Created by Wei00161@umn.edu on 2020/5/15.
//

#ifndef LEVELDB_HM_ZONE_H
#define LEVELDB_HM_ZONE_H
#include "zone_namespace.h"
//TODO write pointer position look up
namespace leveldb {

    class HmZone: public Zone {
    public:
        HmZone()= default;
        HmZone(size_t id){
            zone_info_.id = id;
        }
        ~HmZone()= default;

        Status OpenZone(){

        }

        Status CloseZone();

        Status FinishZone();

        ZoneInfo ReportZone();

        Status ResetWritePointer();

        Status Read();

        Status Write();

    };



    class HmZoneNamespace: public ZoneNamespace {
    public:
        HmZoneNamespace(){
        }
        ~HmZoneNamespace();


        // Create a new zone.
        Status NewZone(){
            Status status;
            HmZone hmZone(next_zone_id_);
            auto it = zones_.emplace(std::make_pair(next_zone_id_,hmZone));
            ++next_zone_id_;
            return status;
        }

    };

}
#endif //LEVELDB_HM_ZONE_H
