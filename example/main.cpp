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

  pqrs::osx::frontmost_application_monitor::monitor::initialize_shared_monitor(dispatcher);

  auto weak_monitor = pqrs::osx::frontmost_application_monitor::monitor::get_shared_monitor();

  if (auto m = weak_monitor.lock()) {
    m->frontmost_application_changed.connect([](auto&& application_ptr) {
      if (application_ptr) {
        if (auto& bundle_identifier = application_ptr->get_bundle_identifier()) {
          std::cout << "bundle_identifier: " << *bundle_identifier << std::endl;
        }
        if (auto& bundle_path = application_ptr->get_bundle_path()) {
          std::cout << "bundle_path: " << *bundle_path << std::endl;
        }
        if (auto& file_path = application_ptr->get_file_path()) {
          std::cout << "file_path: " << *file_path << std::endl;
        }
        if (auto& pid = application_ptr->get_pid()) {
          std::cout << "pid: " << *pid << std::endl;
        }

        std::cout << std::endl;
      }
    });

    m->frontmost_application_changed.connect([](auto&& application_ptr) {
      if (application_ptr) {
        if (auto& bundle_identifier = application_ptr->get_bundle_identifier()) {
          std::cout << "bundle_identifier (the second invocation): " << *bundle_identifier << std::endl;
        }

        std::cout << std::endl;
      }
    });

    m->trigger();
  }

  std::thread thread([] {
    global_wait->wait_notice();

    pqrs::osx::frontmost_application_monitor::monitor::terminate_shared_monitor();

    CFRunLoopStop(CFRunLoopGetMain());
  });

  // ============================================================

  CFRunLoopRun();

  // ============================================================

  thread.join();

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
