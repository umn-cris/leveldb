//
// Created by Zhichao Cao czc199182@gmail.com 07/28/2020.
//

#include "zns_file_writer.h"

namespace leveldb {

  Status ZnsFileWriterManager::CreateFileByThisWriter(uint64_t now_time,
      ZnsFileWriter* writer, std::string file_name) {
    if (writer == nullptr) {
      return Status::Corruption("no file writer");
    }
    auto found = file_to_writer_.find(file_name);
    if (found != file_to_writer_.end()) {
      found->second.create_time = now_time;
      found->second.delete_time = 0;
      found->second.writer_ptr->RemoveFile(file_name);

      found->second.writer_ptr = writer;
      writer->CreateFile(file_name);
      return Status::OK();
    }
    FileInfoInWriter info;
    info.create_time = now_time;
    info.delete_time = 0;
    info.writer_ptr = writer;
    file_to_writer_.insert(std::make_pair(file_name, info));
    writer->CreateFile(file_name);
    return Status::OK();
  }

 //should be called together with ZoneMapping::DeleteFileOnZone
  Status ZnsFileWriterManager::DeleteFile(uint64_t now_time, std::string file_name) {
    auto found = file_to_writer_.find(file_name);
    if (found == file_to_writer_.end()) {
      return Status::OK();
    }
    found->second.delete_time = now_time;
    auto ptr = found->second.writer_ptr;
    ptr->RemoveFile(file_name);
    return Status::OK();
  }

  ZnsFileWriter* ZnsFileWriterManager::GetZnsFileWriter(WriteHints hints) {
    if (hints.write_level < 0) {
      return nullptr;
    }
    if(file_writer_map_.find(hints.write_level) == file_writer_map_.end()) {
      AddFileWriter(hints.write_level, INT_MAX);
    }
    auto sub_writers = file_writer_map_[hints.write_level];
    int score = INT_MAX;   // to be replaced by the real score
    ZnsFileWriter* ret = nullptr;
    for (auto i = sub_writers.begin(); i != sub_writers.end(); i++) {
      if (score >= i->second->GetScore()) {
        ret = i->second;
        break;
      }
    }
    if (ret == nullptr) {
      ret = sub_writers.begin()->second;
    }
    return ret;
  }

  Status ZnsFileWriterManager::AddFileWriter(int level, int score) {
    ZnsZoneInfo* z_info;
    zone_mapping_->GetAndUseOneEmptyZone(&z_info);
    if (z_info == nullptr) {
      return Status::NotFound("not find empty zone");
    }
    ZnsFileWriter file_writer(z_info, level, score);
    file_writers_.push_back(file_writer);
    file_writer_map_[level][score] = &(file_writers_.back());
    return Status::OK();
  }

ZnsFileWriterManager* GetDefualtZnsFileWriterManager() {
  static ZnsFileWriterManager* zfm_ptr = new ZnsFileWriterManager(GetDefaultZoneMapping());
  return zfm_ptr;
}


} //name space
