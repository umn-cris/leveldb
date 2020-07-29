//
// Created by Zhichao Cao czc199182@gmail.com 07/28/2020.
//

#ifndef LEVELDB_ZNS_FILE_WRITER_H
#define LEVELDB_ZNS_FILE_WRITER_H

#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <functional>

#include "leveldb/env.h"
#include "zone_namespace.h"
#include "zone_mapping.h"

namespace leveldb
{

class ZnsFileWriter {
 public:
  ZnsFileWriter(ZnsZoneInfo* new_open_zone, int level, int score) {
    SetOpenZone(new_open_zone);
    is_in_use_ = false;
    level_ = level;
    score_ = score;
  }

  ~ZnsFileWriter() {}

  int GetZoneID() { return open_zone_->zone_id; }

  Status SetOpenZone(ZnsZoneInfo* new_open_zone) {
    open_zone_ = new_open_zone;
    return Status::OK();
  }

  void SetInUse() { is_in_use_ = true; }

  void SetToAvailable() { is_in_use_ = false; }

  bool GetUsageStatus() { return is_in_use_; }

  void CreateFile(std::string file_name) {
    live_files_.insert(file_name);
  }

  void RemoveFile(std::string file_name) {
    live_files_.erase(file_name);
  }

  int GetScore() {
    return score_;
  }

  void SetScore(int score) {
    score_ = score;
  }

 private:
  ZnsZoneInfo* open_zone_;
  std::set<std::string> live_files_;
  bool is_in_use_;
  int level_;
  int score_;
};

struct FileInfoInWriter {
  ZnsFileWriter* writer_ptr;
  uint64_t create_time;
  uint64_t delete_time;
};

class ZnsFileWriterManager {
 public:
  ZnsFileWriterManager(ZoneMapping* zone_mapping) {
    zone_mapping_ = zone_mapping;
    int logger_num = 5;
    for (int i=0; i< logger_num; i++) {
      AddFileWriter(i, INT_MAX);
    }
  }

  ~ZnsFileWriterManager() {}

  Status CreateFileByThisWriter(uint64_t now_time, ZnsFileWriter* writer,
              std::string file_name);

 //should be called together with ZoneMapping::DeleteFileOnZone
  Status DeleteFile(uint64_t now_time, std::string file_name);

  ZnsFileWriter* GetZnsFileWriter(WriteHints hints);

  Status AddFileWriter(int level, int score);

 private:
  ZoneMapping* zone_mapping_;
  std::unordered_map<std::string, FileInfoInWriter> file_to_writer_;
  std::unordered_map<int, std::map<int, ZnsFileWriter*, std::greater<int>>> file_writer_map_;
  std::vector<ZnsFileWriter> file_writers_;

};

extern ZnsFileWriterManager* GetDefualtZnsFileWriterManager();

} // namespace leveldb
#endif
