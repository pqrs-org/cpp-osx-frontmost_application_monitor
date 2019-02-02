#include <catch2/catch.hpp>

#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/boost.hpp>

TEST_CASE("boost application") {
  {
    pqrs::osx::frontmost_application_monitor::application application;
    application.set_bundle_identifier("com.apple.finder");
    application.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");

    REQUIRE(std::hash<pqrs::osx::frontmost_application_monitor::application>{}(application) ==
            pqrs::osx::frontmost_application_monitor::hash_value(application));
  }
}
