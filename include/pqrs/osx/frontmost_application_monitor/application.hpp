#pragma once

// (C) Copyright Takayama Fumihiko 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <optional>
#include <string>

namespace pqrs {
namespace osx {
namespace frontmost_application_monitor {
class application final {
public:
  application(const char* bundle_identifier,
              const char* file_path) {
    if (bundle_identifier) {
      bundle_identifier_ = bundle_identifier;
    }
    if (file_path) {
      file_path_ = file_path;
    }
  }

  const std::optional<std::string>& get_bundle_identifier(void) const {
    return bundle_identifier_;
  }

  const std::optional<std::string>& get_file_path(void) const {
    return file_path_;
  }

private:
  std::optional<std::string> bundle_identifier_;
  std::optional<std::string> file_path_;
};
} // namespace frontmost_application_monitor
} // namespace osx
} // namespace pqrs

namespace std {
template <>
struct hash<pqrs::osx::frontmost_application_monitor::application> final {
  std::size_t operator()(const pqrs::osx::frontmost_application_monitor::application& value) const {
    size_t h = 0;

    if (auto& bundle_identifier = value.get_bundle_identifier()) {
      h = std::hash<std::string>{}(*bundle_identifier);
      h <<= 1;
    }

    if (auto& file_path = value.get_file_path()) {
      h = std::hash<std::string>{}(*file_path);
      h <<= 1;
    }

    return h;
  }
};
} // namespace std
