//
// Created by Wei00161@umn.edu on 2020/5/15.
//

//
// Created by Wei00161@umn.edu on 2020/5/14.
//

#include "leveldb/db.h"
#include "leveldb/env_zns.h"
#include <cassert>
#include <iostream>
#include "hm_zone.h"
using namespace std;
using namespace leveldb;

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
    assert(status.ok());

    status = db->Put(WriteOptions(), "KeyNameExample", "ValueExample");
    assert(status.ok());
    string res;
    status = db->Get(ReadOptions(), "KeyNameExample", &res);
    assert(status.ok());
    cout << res << endl;

    HmZoneNamespace hmZoneNamespace;
    hmZoneNamespace.NewZone();

    ZoneAddress zoneAddress;
    zoneAddress.zone_id = 0;
    zoneAddress.offset = 0;
    const char test[] = "this is a test";
    zoneAddress.length = sizeof(test);
    cout<<"~~~~~~write~~~~~~~~"<<endl;
    status = hmZoneNamespace.Write(zoneAddress,test);
    cout<<status.ToString()<<endl;
    cout<<"~~~~~~~~~~~read~~~~~~~~~~~"<<endl;
    char result[zoneAddress.length];
    hmZoneNamespace.Read(zoneAddress,result);
    cout<<status.ToString()<<endl;
    cout<<result<<endl;


    delete db;
    return 0;
}