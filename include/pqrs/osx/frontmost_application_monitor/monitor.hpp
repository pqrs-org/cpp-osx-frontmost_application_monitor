#pragma once

// (C) Copyright Takayama Fumihiko 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See https://www.boost.org/LICENSE_1_0.txt)

// `pqrs::osx::frontmost_application_monitor` can be used safely in a multi-threaded environment.

#include "application.hpp"
#include "impl/impl.h"
#include <nod/nod.hpp>
#include <pqrs/dispatcher.hpp>

namespace pqrs {
namespace osx {
namespace frontmost_application_monitor {
class monitor final : public dispatcher::extra::dispatcher_client {
public:
  // Signals (invoked from the dispatcher thread)

  nod::signal<void(std::shared_ptr<application>)> frontmost_application_changed;

  // Methods

private:
  monitor(const monitor&) = delete;

  monitor(std::weak_ptr<dispatcher::dispatcher> weak_dispatcher) : dispatcher_client(weak_dispatcher) {
    enqueue_to_dispatcher([] {
      pqrs_osx_frontmost_application_monitor_register(static_cpp_callback);
    });
  }

public:
  virtual ~monitor(void) {
    detach_from_dispatcher([] {
      pqrs_osx_frontmost_application_monitor_unregister(static_cpp_callback);
    });
  }

  static void initialize_shared_monitor(std::weak_ptr<dispatcher::dispatcher> weak_dispatcher) {
    std::lock_guard<std::mutex> guard(mutex_);

    shared_monitor_ = std::shared_ptr<monitor>(new monitor(weak_dispatcher));
  }

  static void terminate_shared_monitor(void) {
    std::lock_guard<std::mutex> guard(mutex_);

    shared_monitor_ = nullptr;
  }

  static std::shared_ptr<monitor> get_shared_monitor(void) {
    std::lock_guard<std::mutex> guard(mutex_);

    return shared_monitor_;
  }

private:
  static void static_cpp_callback(const char* bundle_identifier,
                                  const char* file_path) {
    auto m = shared_monitor_;
    if (m) {
      m->cpp_callback(bundle_identifier,
                      file_path);
    }
  }

  void cpp_callback(const char* bundle_identifier,
                    const char* file_path) {
    auto application_ptr = std::make_shared<application>();
    if (bundle_identifier) {
      application_ptr->set_bundle_identifier(bundle_identifier);
    }
    if (file_path) {
      application_ptr->set_file_path(file_path);
    }

    enqueue_to_dispatcher([this, application_ptr] {
      frontmost_application_changed(application_ptr);
    });
  }

  static inline std::shared_ptr<monitor> shared_monitor_;
  static inline std::mutex mutex_;
};
} // namespace frontmost_application_monitor
} // namespace osx
} // namespace pqrs
