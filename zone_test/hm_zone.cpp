//
// Created by Wei00161@umn.edu on 2020/5/16.
//

#include <sys/stat.h>
#include <dirent.h>
#include "hm_zone.h"

using namespace std;
using namespace leveldb;

void initZone(char* filename, HmZone& hmZone){
    //read the header of a zone (file) to recover it
    // return the zone to ZNS's map
}
void initZNS(const char * dir_name, map<int, HmZone> zone_list){
    // check if dir_name is a valid dir
    struct stat s;
    lstat( dir_name , &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        //creat dir
        int success = mkdir(dir_name, S_IRWXU);
        if(success == 0) cout<<"[hm_zone.cpp] [init_scanner] first time open this ZNS, create dir "<<dir_name<<endl;
        if(success == -1) cout<<"[hm_zone.cpp] [init_scanner] first time open this ZNS. However, create dir "<<dir_name<<" failed"<<endl;
        return;
    }
    // recover file list
    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir( dir_name );
    if( NULL == dir )
    {
        cout<<"[hm_zone.cpp] [init_scanner] Can not open dir "<<dir_name<<endl;
        return;
    }
    zone_list.clear();
    while( ( filename = readdir(dir) ) != NULL )
    {
        // get rid of "." and ".."
        if( strcmp( filename->d_name , "." ) == 0 ||
            strcmp( filename->d_name , "..") == 0    )
            continue;
        int zone_id = atol(filename->d_name);
        HmZone hmZone;
        initZone(filename->d_name, hmZone);
        zone_list.emplace(zone_id,filename ->d_name);
    }
    return;
}

HmZoneNamespace::HmZoneNamespace() {
    string path = "hm_zones_";
    initZNS(path.c_str(),zones_);
}
