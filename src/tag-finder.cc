#include "tag-finder.h"

void TagFinder::Execute() {
  tagEntry entry;
  if (tagsFind(file, &entry, tag.data(), options) == TagSuccess) {
    matches.push_back(Tag(entry));
    while (tagsFindNext(file, &entry) == TagSuccess)
      matches.push_back(Tag(entry));
  }
}

void TagFinder::OnOK() {
  Napi::HandleScope handle_scope(Env());
  Napi::Array array = Napi::Array::New(Env(), matches.size());
  for (size_t i = 0; i < matches.size(); i++) {
    Napi::Object tagObject = Napi::Object::New(Env());
    tagObject.Set("name", matches[i].name);
    tagObject.Set("file", matches[i].file);
    tagObject.Set("kind", matches[i].kind);
    tagObject.Set("lineNumber", Napi::Number::New(Env(), matches[i].lineNumber));
    if (matches[i].pattern.length() > 0)
      tagObject.Set("pattern", matches[i].pattern);
    array.Set(i, tagObject);
  }

  Callback().Call({Env().Null(), array});
}
