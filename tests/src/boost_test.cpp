#include <catch2/catch.hpp>

#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/boost.hpp>

TEST_CASE("boost application") {
  {
    const char* bundle_identifier = "com.apple.finder";
    const char* file_path = "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder";
    pqrs::osx::frontmost_application_monitor::application application1(bundle_identifier,
                                                                       file_path);

    pqrs::osx::frontmost_application_monitor::application application2;

    REQUIRE(boost::hash_value(application1) != boost::hash_value(application2));
  }
}
