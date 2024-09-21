#include <boost/ut.hpp>
#include <iostream>
#include <pqrs/osx/frontmost_application_monitor.hpp>

void run_monitor_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "monitor"_test = [] {
    auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
    auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

    for (int i = 0; i < 10000; ++i) {
      auto wait = pqrs::make_thread_wait();

      pqrs::osx::frontmost_application_monitor::monitor::initialize_shared_monitor(dispatcher);

      auto weak_monitor = pqrs::osx::frontmost_application_monitor::monitor::get_shared_monitor();

      if (auto m = weak_monitor.lock()) {
        m->frontmost_application_changed.connect([wait](auto&& application_ptr) {
          wait->notify();
        });

        m->trigger();
      }

      wait->wait_notice();

      pqrs::osx::frontmost_application_monitor::monitor::terminate_shared_monitor();
    }

    dispatcher->terminate();
    dispatcher = nullptr;
  };
}
