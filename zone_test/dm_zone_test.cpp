//
// Created by Wei00161@umn.edu on 2020/5/14.
//

#include "leveldb/db.h"
#include <cassert>
#include <iostream>
#include "zone_namespace.h"
#include "dm_zone.h"
using namespace std;
using namespace leveldb;

int main() {

    ZoneAddress zoneAddress;
    zoneAddress.zone_id = 0;
    zoneAddress.offset = 0;
    const char test[] = "that is a test";
    zoneAddress.length = sizeof(test);
    cout<<"~~~~~~write~~~~~~~~"<<endl;
    shared_ptr<DmZoneNamespace> dmzonenamespace = DmZoneNamespace::CreatZoneNamespace();
    shared_ptr<Zone> dmzone = dmzonenamespace->GetZone(0);
    dmzone->ZoneWrite(zoneAddress,test);

    cout<<"~~~~~~~~~~~read~~~~~~~~~~~"<<endl;
    char result[zoneAddress.length];
    dmzone->ZoneRead(zoneAddress,result);
    cout<<result<<endl;


    return 0;
}
