#include <Carbon/Carbon.h>
#include <csignal>
#include <iostream>
#include <pqrs/osx/frontmost_application_monitor.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  auto monitor = std::make_shared<pqrs::osx::frontmost_application_monitor::monitor>(dispatcher);

  monitor->frontmost_application_changed.connect([](auto&& application_ptr) {
    if (application_ptr) {
      std::cout << "frontmost_application_changed" << std::endl;

      if (auto& bundle_identifier = application_ptr->get_bundle_identifier()) {
        std::cout << "  bundle_identifier: " << *bundle_identifier << std::endl;
      }
      if (auto& file_path = application_ptr->get_file_path()) {
        std::cout << "  file_path: " << *file_path << std::endl;
      }

      std::cout << std::endl;
    }
  });

  monitor->async_start();

  std::thread thread([&monitor] {
    global_wait->wait_notice();

    // Destroy monitor before `CFRunLoopStop(CFRunLoopGetMain())`.
    monitor = nullptr;

    CFRunLoopStop(CFRunLoopGetMain());
  });

  // ============================================================

  CFRunLoopRun();

  // ============================================================

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
