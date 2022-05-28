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

      auto monitor = std::make_shared<pqrs::osx::frontmost_application_monitor::monitor>(dispatcher);

      monitor->frontmost_application_changed.connect([wait](auto&& application_ptr) {
        wait->notify();
      });

      monitor->async_start();

      wait->wait_notice();

      monitor = nullptr;
    }

    dispatcher->terminate();
    dispatcher = nullptr;
  };
}
