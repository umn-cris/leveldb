#ifndef ZONE_NAMESPACE_H_
#define ZONE_NAMESPACE_H_

#include "leveldb/status.h"

namespace leveldb {

enum ZoneType {CONVENTIONAL, SEQUENTIAL_WRITE_PREFERRED, SEQUENTIAL_WRITE_REQUIRED};
enum ZoneCondition {OPEN, CLOSED};
enum ZoneState {SEQUENTIAL, NON_SEQUENTIAL};

class ZoneInfo {
public:
    int size;
    int write_pointer;
    int first_LBA;
    ZoneType zone_type;
    ZoneCondition zone_condition;
    ZoneState zone_state;
};

class ZoneNamespace {
public:
    ZoneNamespace() = delete;
    virtual ~ZoneNamespace();

    virtual Status OpenZone() = 0;

    virtual Status CloseZone() = 0;

    virtual Status FinishZone() = 0;

    virtual ZoneInfo ReportZone() = 0;

    virtual Status ResetWritePointer() = 0;

    virtual Status Read() = 0;

    virtual Status Write() = 0;

private:

    ZoneInfo zone_info_; 
};

}

#endif // ZONE_NAMESPACE_H_