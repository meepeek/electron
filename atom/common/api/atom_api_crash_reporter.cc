// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include <map>
#include <string>

#include "atom/common/crash_reporter/crash_reporter.h"
#include "base/bind.h"
#include "native_mate/dictionary.h"

#include "atom/common/node_includes.h"

namespace mate {

template<>
struct Converter<std::map<std::string, std::string> > {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     std::map<std::string, std::string>* out) {
    if (!val->IsObject())
      return false;

    v8::Local<v8::Object> dict = val->ToObject();
    v8::Local<v8::Array> keys = dict->GetOwnPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); ++i) {
      v8::Local<v8::Value> key = keys->Get(i);
      (*out)[V8ToString(key)] = V8ToString(dict->Get(key));
    }
    return true;
  }
};

}  // namespace mate

namespace {

void Initialize(v8::Local<v8::Object> exports, v8::Local<v8::Value> unused,
                v8::Local<v8::Context> context, void* priv) {
  using crash_reporter::CrashReporter;
  mate::Dictionary dict(context->GetIsolate(), exports);
  dict.SetMethod("start",
                 base::Bind(&CrashReporter::Start,
                            base::Unretained(CrashReporter::GetInstance())));
}

}  // namespace

NODE_MODULE_CONTEXT_AWARE_BUILTIN(atom_common_crash_reporter, Initialize)
