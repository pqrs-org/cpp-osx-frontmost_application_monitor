#include <catch2/catch.hpp>

#include <pqrs/osx/frontmost_application_monitor.hpp>

TEST_CASE("monitor") {
  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  auto monitor = std::make_shared<pqrs::osx::frontmost_application_monitor::monitor>(dispatcher);

  monitor = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;
}
