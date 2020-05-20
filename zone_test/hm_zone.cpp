//
// Created by Wei00161@umn.edu on 2020/5/16.
//

#include <sys/stat.h>
#include <dirent.h>
#include "hm_zone.h"

using namespace std;
using namespace leveldb;
string path = "hm_zones_";

HmZone::HmZone(fstream& fs,size_t id):modify_zone_(fs){
    //zone_info_ = (struct ZoneInfo*)malloc(sizeof(struct ZoneInfo));
    zoneInfo_.id = id;
    zoneInfo_.write_pointer = 0;
    zoneInfo_.zone_type = SEQUENTIAL_WRITE_REQUIRED;
    zoneInfo_.zone_condition = CLOSED;
    zoneInfo_.zone_state = SEQUENTIAL;
    modify_zone_.open(path+"/"+to_string(zoneInfo_.id),ios::out);
    modify_zone_<<ToString()<<endl;
    modify_zone_.close();
    cout<<"create zone"<<endl;
}


Status HmZone::OpenZone() {
    Status status;
    modify_zone_.open(path+"/"+to_string(zoneInfo_.id), ios::out|ios::in);
    if(!modify_zone_.is_open()){
        status = Status::NotFound("Open zone failed");
        cout<<"[hm_zone.cpp] [OpenZone] open zone "<<zoneInfo_.id<<" failed"<<endl;
    }
    zoneInfo_.zone_condition = OPEN;
    modify_zone_.seekp(zoneInfo_.write_pointer);
    status = Status::OK();
    cout<<"open zone"<<endl;
    return status;
}

Status HmZone::CloseZone() {
    Status status;
    zoneInfo_.zone_condition=CLOSED;
    modify_zone_.seekp(0,ios::beg);
    modify_zone_<<ToString()<<endl;
    modify_zone_.close();
    status = Status::OK();
    cout<<"close zone"<<endl;
    return status;
}

Status HmZone::FinishZone() {
    Status status;
    zoneInfo_.write_pointer = ZONESIZE;
    zoneInfo_.size = ZONESIZE;
    modify_zone_.seekp(0,ios::beg);
    modify_zone_<<ToString()<<endl;

    // no close here!
    status = Status::OK();
    cout<<"finishe zone"<<endl;
    return status;
}

ZoneInfo HmZone::ReportZone() {
    cout<<"report zone"<<endl;
    return zoneInfo_;
}

Status HmZone:: ResetWritePointer(){
    Status status;
    zoneInfo_.write_pointer = 0;
    zoneInfo_.size = 0;
    modify_zone_.seekp(0,ios::beg);
    modify_zone_<<ToString()<<endl;
    // no close here!
    status = Status::OK();
    cout<<"reset write pointer"<<endl;
    return status;
}

Status HmZoneNamespace::NewZone(){
    Status status;
    fstream fs;
    auto it = zones_.emplace(next_zone_id_, HmZone(fs,next_zone_id_));
    ++next_zone_id_;
    return status;
}
Status HmZoneNamespace:: InitZone(const char *path, const char *filename,  char *filepath){
    strcpy(filepath, path);
    if(filepath[strlen(path) - 1] != '/')
        strcat(filepath, "/");
    strcat(filepath, filename);
    printf("[hm_zone.cpp] [InitZone] path is = %s\n",filepath);
}
//delete all zone files left, create a new zone env
Status HmZoneNamespace::InitZNS(const char * dir_name){
    // check if dir_name is a valid dir
    Status status;
    char filepath[256] = {0};
    struct stat s;
    lstat( dir_name , &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        //creat dir
        int success = mkdir(dir_name, S_IRWXU);
        if(success == 0) {
            cout<<"[hm_zone.cpp] [init_scanner] first time open this ZNS, create dir "<<dir_name<<endl;
            status = Status::OK();
        }
        if(success == -1){
            cout<<"[hm_zone.cpp] [init_scanner] first time open this ZNS. However, create dir "<<dir_name<<" failed"<<endl;
            status = Status::NotSupported("dirctory create filed");
        }
        return status;
    }
    // recover file list
    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir( dir_name );
    if( NULL == dir )
    {
        cout<<"[hm_zone.cpp] [init_scanner] Can not open dir "<<dir_name<<endl;
        status = Status::NotFound("dirctory open filed");
        return status;
    }
    zones_.clear();
    while( ( filename = readdir(dir) ) != NULL )
    {
        InitZone(dir_name, filename->d_name, filepath);
        // get rid of "." and ".."
        if( strcmp( filename->d_name , "." ) == 0 ||
            strcmp( filename->d_name , "..") == 0    )
            continue;
        if( remove(filepath) == 0 )
            printf("Removed %s.\n", filepath);
        else
            perror("remove fail");
        /*int zone_id = atol(filename->d_name);
        fstream fs;
        HmZone hmZone(fs);
        InitZone(filename->d_name, hmZone);
        zones_.emplace(zone_id,hmZone);*/
    }
    status = Status::OK();
    return status;
}

HmZoneNamespace::HmZoneNamespace() {
    InitZNS(path.c_str());
}
    