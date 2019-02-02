#include <catch2/catch.hpp>

#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/nlohmann_json.hpp>

TEST_CASE("nlohmann_json application") {
  {
    pqrs::osx::frontmost_application_monitor::application application1;
    application1.set_bundle_identifier("com.apple.finder");
    application1.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");

    nlohmann::json json = application1;

    auto application2 = json.get<pqrs::osx::frontmost_application_monitor::application>();

    REQUIRE(application1 == application2);
  }
}
