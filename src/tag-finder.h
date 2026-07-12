#ifndef SRC_TAG_FINDER_H_
#define SRC_TAG_FINDER_H_

#include <string>
#include <vector>
#include <napi.h>
#include "tag.h"
#include "readtags.h"

class TagFinder : public Napi::AsyncWorker {
 public:
  TagFinder(const Napi::Function& callback, std::string tag, int options, tagFile* file)
    : Napi::AsyncWorker(callback), options(options), tag(tag), file(file) {}

  void Execute() override;
  void OnOK() override;

 private:
  int options;
  std::string tag;
  std::vector< Tag > matches;
  tagFile *file;
};

#endif  // SRC_TAG_FINDER_H_
