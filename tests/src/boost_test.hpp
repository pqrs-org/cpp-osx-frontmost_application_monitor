#include <boost/ut.hpp>
#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/boost.hpp>

void run_boost_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "boost application"_test = [] {
    {
      pqrs::osx::frontmost_application_monitor::application application;
      application.set_bundle_identifier("com.apple.finder");
      application.set_bundle_path("/System/Library/CoreServices/Finder.app");
      application.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      application.set_pid(42);

      expect(std::hash<pqrs::osx::frontmost_application_monitor::application>{}(application) ==
             pqrs::osx::frontmost_application_monitor::hash_value(application));
    }
  };
}
