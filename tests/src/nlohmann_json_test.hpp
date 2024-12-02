#include <boost/ut.hpp>
#include <iostream>
#include <pqrs/osx/frontmost_application_monitor/extra/nlohmann_json.hpp>

void run_nlohmann_json_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "nlohmann_json application"_test = [] {
    {
      pqrs::osx::frontmost_application_monitor::application application1;
      application1.set_bundle_identifier("com.apple.finder");
      application1.set_bundle_path("/System/Library/CoreServices/Finder.app");
      application1.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      application1.set_pid(42);

      nlohmann::json json = application1;

      auto application2 = json.get<pqrs::osx::frontmost_application_monitor::application>();

      expect(application1 == application2);
    }

    // from_json

    {
      std::string bundle_identifier("com.apple.finder");
      std::string bundle_path("/System/Library/CoreServices/Finder.app");
      std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      pid_t pid(42);

      auto json = nlohmann::json::object(
          {
              {"bundle_identifier", bundle_identifier},
              {"bundle_path", bundle_path},
              {"file_path", file_path},
              {"pid", pid},
          });
      auto application = json.get<pqrs::osx::frontmost_application_monitor::application>();

      expect(application.get_bundle_identifier() == bundle_identifier);
      expect(application.get_bundle_path() == bundle_path);
      expect(application.get_file_path() == file_path);
      expect(application.get_pid() == pid);
    }

    // from_json (empty)

    {
      auto application = nlohmann::json::object().get<pqrs::osx::frontmost_application_monitor::application>();

      expect(application.get_bundle_identifier() == std::nullopt);
      expect(application.get_bundle_path() == std::nullopt);
      expect(application.get_file_path() == std::nullopt);
      expect(application.get_pid() == std::nullopt);
    }

    // type error

    try {
      nlohmann::json().get<pqrs::osx::frontmost_application_monitor::application>();
      expect(false);
    } catch (pqrs::json::unmarshal_error& ex) {
      expect(std::string("json must be object, but is `null`") == ex.what());
    } catch (...) {
      expect(false);
    }

    // unknown key

    {
      auto json = nlohmann::json::object(
          {
              {"bundle-identifier", "com.apple.finder"},
              {"bundle_path", "/System/Library/CoreServices/Finder.app"},
              {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
              {"pid", 42},
          });
      try {
        json.get<pqrs::osx::frontmost_application_monitor::application>();
        expect(false);
      } catch (pqrs::json::unmarshal_error& ex) {
        expect(std::string("unknown key: `bundle-identifier`") == ex.what());
      } catch (...) {
        expect(false);
      }
    }

    // type error (bundle_identifier)

    {
      auto json = nlohmann::json::object(
          {
              {"bundle_identifier", nlohmann::json()},
              {"bundle_path", "/System/Library/CoreServices/Finder.app"},
              {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
              {"pid", 42},
          });
      try {
        json.get<pqrs::osx::frontmost_application_monitor::application>();
        expect(false);
      } catch (pqrs::json::unmarshal_error& ex) {
        expect(std::string("`bundle_identifier` must be string, but is `null`") == ex.what());
      } catch (...) {
        expect(false);
      }
    }

    // type error (bundle_path)

    {
      auto json = nlohmann::json::object(
          {
              {"bundle_identifier", "com.apple.finder"},
              {"bundle_path", nlohmann::json()},
              {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
              {"pid", 42},
          });
      try {
        json.get<pqrs::osx::frontmost_application_monitor::application>();
        expect(false);
      } catch (pqrs::json::unmarshal_error& ex) {
        expect(std::string("`bundle_path` must be string, but is `null`") == ex.what());
      } catch (...) {
        expect(false);
      }
    }

    // type error (file_path)

    {
      auto json = nlohmann::json::object(
          {
              {"bundle_identifier", "com.apple.finder"},
              {"bundle_path", "/System/Library/CoreServices/Finder.app"},
              {"file_path", nlohmann::json()},
              {"pid", 42},
          });
      try {
        json.get<pqrs::osx::frontmost_application_monitor::application>();
        expect(false);
      } catch (pqrs::json::unmarshal_error& ex) {
        expect(std::string("`file_path` must be string, but is `null`") == ex.what());
      } catch (...) {
        expect(false);
      }
    }

    // type error (pid)

    {
      auto json = nlohmann::json::object(
          {
              {"bundle_identifier", "com.apple.finder"},
              {"bundle_path", "/System/Library/CoreServices/Finder.app"},
              {"file_path", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder"},
              {"pid", nlohmann::json()},
          });
      try {
        json.get<pqrs::osx::frontmost_application_monitor::application>();
        expect(false);
      } catch (pqrs::json::unmarshal_error& ex) {
        expect(std::string("`pid` must be number, but is `null`") == ex.what());
      } catch (...) {
        expect(false);
      }
    }
  };
}
