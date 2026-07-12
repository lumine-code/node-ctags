#include "tag-reader.h"

void TagReader::Execute() {
  tags.clear();

  for (int i = 0; i < chunkSize; i++) {
    tagEntry entry;
    if (tagsNext(file, &entry) == TagSuccess)
      tags.push_back(Tag(entry));
    else
      break;
  }
}

void TagReader::OnOK() {
  Napi::HandleScope handle_scope(Env());
  Napi::Array array = Napi::Array::New(Env(), tags.size());
  for (size_t i = 0; i < tags.size(); i++) {
    Napi::Object tagObject = Napi::Object::New(Env());
    tagObject.Set("name", tags[i].name);
    tagObject.Set("file", tags[i].file);
    tagObject.Set("kind", tags[i].kind);
    tagObject.Set("lineNumber", Napi::Number::New(Env(), tags[i].lineNumber));
    if (tags[i].pattern.length() > 0)
      tagObject.Set("pattern", tags[i].pattern);
    array.Set(i, tagObject);
  }

  Callback().Call({Env().Null(), array});
}
