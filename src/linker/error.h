/*
 * Copyright 2018 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WABT_ERROR_H_
#define WABT_ERROR_H_

#include "StringRef.h"
#include <string>
#include <vector>

#include "common.h"

namespace wabt {

enum class ErrorLevel {
    Warning,
    Error,
};

static inline const char *GetErrorLevelName(ErrorLevel error_level) {
    switch (error_level) {
    case ErrorLevel::Warning:
        return "warning";
    case ErrorLevel::Error:
        return "error";
    }
    WABT_UNREACHABLE;
}

class Error {
public:
    Error() : error_level(ErrorLevel::Error) {
    }
    Error(ErrorLevel error_level, StringRef message)
        : error_level(error_level), message(message.str()) {
    }

    ErrorLevel error_level;
    std::string message;
};

using Errors = std::vector<Error>;

} // namespace wabt

#endif //  WABT_ERROR_H_
