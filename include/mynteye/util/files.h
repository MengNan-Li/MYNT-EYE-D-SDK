// Copyright 2018 Slightech Co., Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef MYNTEYE_UTILS_FILES_H_
#define MYNTEYE_UTILS_FILES_H_

#include "mynteye/stubs/global.h"
#include "mynteye/util/strings.h"

#if defined(MYNTEYE_OS_WIN) && !defined(MYNTEYE_OS_MINGW) \
    && !defined(MYNTEYE_OS_CYGWIN)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <string>

MYNTEYE_BEGIN_NAMESPACE

namespace files {

bool _mkdir(const std::string &path) {
#if defined(MYNTEYE_OS_MINGW) || defined(MYNTEYE_OS_CYGWIN)
  const int status = ::mkdir(path.c_str());
#elif defined(MYNTEYE_OS_WIN)
  const int status = ::_mkdir(path.c_str());
#else
  const int status =
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

  if (status != 0 && errno != EEXIST) {
    // std::cout << "Create directory failed (status " << status
    //   << "), path: " << path << std::endl;
    return false;
  }
  if (errno == EEXIST) {
    // std::cout << "Create directory needless"  <<
    // "(already exist), path: " << path << std::endl;
    return true;
  } else {
    // std::cout << "Create directory success, path: " <<
    // path << std::endl;
    return true;
  }
}

bool mkdir(const std::string &path) {
  auto &&dirs = strings::split(path, MYNTEYE_OS_SEP);
  auto &&size = dirs.size();
  if (size <= 0)
    return false;
  std::string p{dirs[0]};
  if (!_mkdir(p))
    return false;
  for (std::size_t i = 1; i < size; i++) {
    p.append(MYNTEYE_OS_SEP).append(dirs[i]);
    if (!_mkdir(p))
      return false;
  }
  return true;
}

}  // namespace files

MYNTEYE_END_NAMESPACE

#endif  // MYNTEYE_UTILS_FILES_H_
