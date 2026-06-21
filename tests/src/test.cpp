#include "application_test.hpp"
#include "boost_test.hpp"
#include "monitor_test.hpp"
#include "nlohmann_json_test.hpp"

int main() {
  run_application_test();
  run_boost_test();
  run_monitor_test();
  run_nlohmann_json_test();
  return 0;
}
