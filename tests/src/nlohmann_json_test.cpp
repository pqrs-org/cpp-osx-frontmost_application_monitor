#include <catch2/catch.hpp>

#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/nlohmann_json.hpp>

TEST_CASE("nlohmann_json application") {
  {
    const char* bundle_identifier = "com.apple.finder";
    const char* file_path = "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder";
    pqrs::osx::frontmost_application_monitor::application application1(bundle_identifier,
                                                                       file_path);

    nlohmann::json json = application1;

    pqrs::osx::frontmost_application_monitor::application application2 = json;

    REQUIRE(application1 == application2);
  }
}
