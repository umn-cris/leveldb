//
// Created by Wei00161@umn.edu on 2020/5/15.
//

//
// Created by Wei00161@umn.edu on 2020/5/14.
//

#include "leveldb/db.h"
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
    fstream fs;
    HmZone test_zone(fs);
    hmZoneNamespace.GetZone(0,test_zone);

    test_zone.OpenZone();
    ZoneInfo info = test_zone.ReportZone();


    test_zone.FinishZone();
    info = test_zone.ReportZone();
    // print whatever you want to check
    //cout<<info.write_pointer<<" "<<info.size<<endl;
    test_zone.ResetWritePointer();
    info = test_zone.ReportZone();
    //cout<<info.write_pointer<<" "<<info.size<<endl;
    test_zone.CloseZone();

    //TODO rerun test program to reopen zone files
    //TODO check ZNS function
    delete db;
    return 0;
}