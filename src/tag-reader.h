#ifndef SRC_TAG_READER_H_
#define SRC_TAG_READER_H_

#include <vector>
#include <napi.h>
#include "tag.h"
#include "readtags.h"

class TagReader : public Napi::AsyncWorker {
 public:
  TagReader(const Napi::Function& callback, int chunkSize, tagFile* file)
    : Napi::AsyncWorker(callback), chunkSize(chunkSize), file(file) {}

  void Execute() override;
  void OnOK() override;

 private:
  int chunkSize;
  std::vector< Tag > tags;
  tagFile *file;
};

#endif  // SRC_TAG_READER_H_
