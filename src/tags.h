#ifndef SRC_TAGS_H_
#define SRC_TAGS_H_

#include <string>
#include <napi.h>
#include "readtags.h"

class Tags : public Napi::ObjectWrap<Tags> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  explicit Tags(const Napi::CallbackInfo& info);
  ~Tags();

 private:
  Napi::Value End(const Napi::CallbackInfo& info);
  Napi::Value Exists(const Napi::CallbackInfo& info);
  Napi::Value FindTags(const Napi::CallbackInfo& info);
  Napi::Value GetTags(const Napi::CallbackInfo& info);

  tagFile* file;
};

#endif  // SRC_TAGS_H_
