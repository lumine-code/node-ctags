#include <string>
#include "tags.h"
#include "tag-finder.h"
#include "tag-reader.h"

Napi::Object Tags::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function constructor = DefineClass(env, "Tags", {
    InstanceMethod("end", &Tags::End),
    InstanceMethod("exists", &Tags::Exists),
    InstanceMethod("findTags", &Tags::FindTags),
    InstanceMethod("getTags", &Tags::GetTags),
  });

  exports.Set("Tags", constructor);
  return exports;
}

Napi::Object InitModule(Napi::Env env, Napi::Object exports) {
  return Tags::Init(env, exports);
}

NODE_API_MODULE(ctags, InitModule)

Tags::Tags(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<Tags>(info), file(nullptr) {
  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(info.Env(), "Path must be a string")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string filePath = info[0].As<Napi::String>().Utf8Value();
  tagFileInfo fileInfo;
  file = tagsOpen(filePath.c_str(), &fileInfo);
  if (!fileInfo.status.opened)
    file = nullptr;
}

Tags::~Tags() {
  if (file != nullptr) {
    tagsClose(file);
    file = nullptr;
  }
}

Napi::Value Tags::GetTags(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsFunction()) {
    Napi::TypeError::New(env, "Expected a chunk size and callback")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  int chunkSize = info[0].As<Napi::Number>().Uint32Value();
  (new TagReader(info[1].As<Napi::Function>(), chunkSize, file))->Queue();
  return env.Undefined();
}

Napi::Value Tags::FindTags(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 4 || !info[0].IsString() || !info[3].IsFunction()) {
    Napi::TypeError::New(env, "Expected a tag, options, and callback")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string tag = info[0].As<Napi::String>().Utf8Value();
  int options = info[1].ToBoolean().Value() ? TAG_PARTIALMATCH : TAG_FULLMATCH;
  options |= info[2].ToBoolean().Value() ? TAG_IGNORECASE : TAG_OBSERVECASE;

  (new TagFinder(info[3].As<Napi::Function>(), tag, options, file))->Queue();
  return env.Undefined();
}

Napi::Value Tags::Exists(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), file != nullptr);
}

Napi::Value Tags::End(const Napi::CallbackInfo& info) {
  if (file != nullptr) {
    tagsClose(file);
    file = nullptr;
  }
  return info.Env().Undefined();
}
