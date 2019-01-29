#include <catch2/catch.hpp>

#include <pqrs/osx/frontmost_application_monitor.hpp>
#include <unordered_set>

TEST_CASE("application") {
  {
    std::string bundle_identifier("com.apple.finder");
    std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
    pqrs::osx::frontmost_application_monitor::application application;
    application.set_bundle_identifier(bundle_identifier);
    application.set_file_path(file_path);
    REQUIRE(application.get_bundle_identifier() == bundle_identifier);
    REQUIRE(application.get_file_path() == file_path);
  }
  {
    pqrs::osx::frontmost_application_monitor::application application;
    REQUIRE(application.get_bundle_identifier() == std::nullopt);
    REQUIRE(application.get_file_path() == std::nullopt);
  }
  {
    std::string bundle_identifier("com.apple.finder");
    std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");

    pqrs::osx::frontmost_application_monitor::application application1;
    application1.set_bundle_identifier(bundle_identifier);
    application1.set_file_path(file_path);

    pqrs::osx::frontmost_application_monitor::application application2;
    application2.set_bundle_identifier(bundle_identifier);

    pqrs::osx::frontmost_application_monitor::application application3;
    application3.set_file_path(file_path);

    pqrs::osx::frontmost_application_monitor::application application4;

    REQUIRE(application1 == application1);
    REQUIRE(application1 != application2);
    REQUIRE(application1 != application3);
    REQUIRE(application1 != application4);
  }
}

TEST_CASE("application hash") {
  std::unordered_set<pqrs::osx::frontmost_application_monitor::application> set;
}
