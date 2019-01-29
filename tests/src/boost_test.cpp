#include <catch2/catch.hpp>

#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/boost.hpp>

TEST_CASE("boost application") {
  {
    pqrs::osx::frontmost_application_monitor::application application1;
    application1.set_bundle_identifier("com.apple.finder");
    application1.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");

    pqrs::osx::frontmost_application_monitor::application application2;

    REQUIRE(pqrs::osx::frontmost_application_monitor::hash_value(application1) !=
            pqrs::osx::frontmost_application_monitor::hash_value(application2));
  }
}
