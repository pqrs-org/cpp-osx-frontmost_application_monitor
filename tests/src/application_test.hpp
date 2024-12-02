#include <boost/ut.hpp>
#include <pqrs/osx/frontmost_application_monitor.hpp>
#include <unordered_set>

void run_application_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "application"_test = [] {
    {
      std::string bundle_identifier("com.apple.finder");
      std::string bundle_path("/System/Library/CoreServices/Finder.app");
      std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      pid_t pid(42);

      pqrs::osx::frontmost_application_monitor::application application;
      application.set_bundle_identifier(bundle_identifier);
      application.set_bundle_path(bundle_path);
      application.set_file_path(file_path);
      application.set_pid(pid);

      expect(application.get_bundle_identifier() == bundle_identifier);
      expect(application.get_bundle_path() == bundle_path);
      expect(application.get_file_path() == file_path);
      expect(application.get_pid() == pid);
    }
    {
      pqrs::osx::frontmost_application_monitor::application application;
      expect(application.get_bundle_identifier() == std::nullopt);
      expect(application.get_bundle_path() == std::nullopt);
      expect(application.get_file_path() == std::nullopt);
      expect(application.get_pid() == std::nullopt);
    }
    {
      std::string bundle_identifier("com.apple.finder");
      std::string bundle_path("/System/Library/CoreServices/Finder.app");
      std::string file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      pid_t pid(42);

      pqrs::osx::frontmost_application_monitor::application application1;
      application1.set_bundle_identifier(bundle_identifier);
      application1.set_bundle_path(bundle_path);
      application1.set_file_path(file_path);
      application1.set_pid(pid);

      pqrs::osx::frontmost_application_monitor::application application2;
      application2.set_bundle_identifier(bundle_identifier);

      pqrs::osx::frontmost_application_monitor::application application3;
      application2.set_bundle_path(bundle_path);

      pqrs::osx::frontmost_application_monitor::application application4;
      application3.set_file_path(file_path);

      pqrs::osx::frontmost_application_monitor::application application5;
      application3.set_pid(pid);

      pqrs::osx::frontmost_application_monitor::application application6;

      expect(application1 == application1);
      expect(application1 != application2);
      expect(application1 != application3);
      expect(application1 != application4);
      expect(application1 != application5);
      expect(application1 != application6);
    }
  };

  "application hash"_test = [] {
    std::unordered_set<pqrs::osx::frontmost_application_monitor::application> set;

    {
      pqrs::osx::frontmost_application_monitor::application application1;
      application1.set_bundle_identifier("com.apple.finder");
      application1.set_bundle_path("/System/Library/CoreServices/Finder.app");
      application1.set_file_path("/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder");
      application1.set_pid(42);

      pqrs::osx::frontmost_application_monitor::application application2;

      expect(std::hash<pqrs::osx::frontmost_application_monitor::application>{}(application1) !=
             std::hash<pqrs::osx::frontmost_application_monitor::application>{}(application2));
    }
  };
}
