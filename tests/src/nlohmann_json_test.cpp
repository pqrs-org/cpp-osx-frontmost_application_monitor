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

  // from_json

  {
    std::string bundle_identifier("com.apple.finder");
    std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");

    auto json = nlohmann::json::object(
        {
            {"bundle_identifier", bundle_identifier},
            {"file_path", file_path},
        });
    auto application = json.get<pqrs::osx::frontmost_application_monitor::application>();

    REQUIRE(application.get_bundle_identifier() == bundle_identifier);
    REQUIRE(application.get_file_path() == file_path);
  }

  // from_json (empty)

  {
    auto application = nlohmann::json::object().get<pqrs::osx::frontmost_application_monitor::application>();

    REQUIRE(application.get_bundle_identifier() == std::nullopt);
    REQUIRE(application.get_file_path() == std::nullopt);
  }

  // type error

  REQUIRE_THROWS_AS(
      nlohmann::json().get<pqrs::osx::frontmost_application_monitor::application>(),
      pqrs::json::unmarshal_error);
  REQUIRE_THROWS_WITH(
      nlohmann::json().get<pqrs::osx::frontmost_application_monitor::application>(),
      "json must be object, but is `null`");

  // unknown key

  {
    auto json = nlohmann::json::object(
        {
            {"bundle-identifier", "com.apple.finder"},
            {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
        });
    REQUIRE_THROWS_AS(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        pqrs::json::unmarshal_error);
    REQUIRE_THROWS_WITH(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        "unknown key: `bundle-identifier`");
  }

  // type error (bundle_identifier)

  {
    auto json = nlohmann::json::object(
        {
            {"bundle_identifier", nlohmann::json()},
            {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
        });
    REQUIRE_THROWS_AS(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        pqrs::json::unmarshal_error);
    REQUIRE_THROWS_WITH(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        "`bundle_identifier` must be string, but is `null`");
  }

  // type error (file_path)

  {
    auto json = nlohmann::json::object(
        {
            {"bundle_identifier", "com.apple.finder"},
            {"file_path", nlohmann::json()},
        });
    REQUIRE_THROWS_AS(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        pqrs::json::unmarshal_error);
    REQUIRE_THROWS_WITH(
        json.get<pqrs::osx::frontmost_application_monitor::application>(),
        "`file_path` must be string, but is `null`");
  }
}
