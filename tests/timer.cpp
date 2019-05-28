#include <utilities/timer.hpp>
#include <catch2/catch.hpp>
#include <thread>
#include <sstream>

TEST_CASE("Timer") {
  std::chrono::milliseconds time2sleep(5);
  utilities::Timer t;
  std::this_thread::sleep_for(time2sleep);
  t.record("sleep 5 ms");
  std::stringstream ss;
  ss<<t<<std::endl;
  auto found = ss.str().find("sleep 5 ms : 0 h 0 m 0 s 5 ms");
  REQUIRE(found != std::string::npos);

}

