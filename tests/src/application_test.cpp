#include <catch2/catch.hpp>

#include <pqrs/osx/frontmost_application_monitor.hpp>
#include <unordered_set>

TEST_CASE("application") {
  {
    const char* bundle_identifier = "com.apple.finder";
    const char* file_path = "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder";
    pqrs::osx::frontmost_application_monitor::application application(bundle_identifier,
                                                                      file_path);
    REQUIRE(application.get_bundle_identifier() == bundle_identifier);
    REQUIRE(application.get_file_path() == file_path);
  }
  {
    const char* bundle_identifier = nullptr;
    const char* file_path = nullptr;
    pqrs::osx::frontmost_application_monitor::application application(bundle_identifier,
                                                                      file_path);
    REQUIRE(application.get_bundle_identifier() == std::nullopt);
    REQUIRE(application.get_file_path() == std::nullopt);
  }
  {
    const char* bundle_identifier = "com.apple.finder";
    const char* file_path = "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder";
    pqrs::osx::frontmost_application_monitor::application application1(bundle_identifier,
                                                                       file_path);
    pqrs::osx::frontmost_application_monitor::application application2(bundle_identifier,
                                                                       nullptr);
    pqrs::osx::frontmost_application_monitor::application application3(nullptr,
                                                                       file_path);
    pqrs::osx::frontmost_application_monitor::application application4(nullptr,
                                                                       nullptr);
    REQUIRE(application1 == application1);
    REQUIRE(application1 != application2);
    REQUIRE(application1 != application3);
    REQUIRE(application1 != application4);
  }
}

TEST_CASE("application hash") {
  std::unordered_set<pqrs::osx::frontmost_application_monitor::application> set;
}
