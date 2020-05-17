//
// Created by Wei00161@umn.edu on 2020/5/14.
//

#ifndef LEVELDB_DM_ZONE_H
#define LEVELDB_DM_ZONE_H

#include "zone_namespace.h"

namespace leveldb {

    class DmZone: public Zone {
    public:
        DmZone();
        ~DmZone();

        Status OpenZone();

        Status CloseZone();

        Status FinishZone();

        ZoneInfo ReportZone();

        Status ResetWritePointer();

        Status Read();

        Status Write();

    };


    
    class DmZoneNamespace: public ZoneNamespace {
    public:
        DmZoneNamespace();
        ~DmZoneNamespace();


        // Create a new zone.
        Status NewZone();

    };

}
#endif //LEVELDB_DM_ZONE_H
