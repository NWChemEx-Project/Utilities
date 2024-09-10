/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "catch.hpp"
#include <sstream>
#include <thread>
#include <utilities/timer.hpp>

TEST_CASE("Timer::record") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.record("sleep 5 ms");
    REQUIRE(t.at("sleep 5 ms") >= time2sleep);
}

TEST_CASE("Timer::reset") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.reset();
    t.record("no sleep");
    REQUIRE(t.at("no sleep") < time2sleep);
}

TEST_CASE("Timer::time_it") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    t.time_it("sleep 5 ms", [=]() { std::this_thread::sleep_for(time2sleep); });
    REQUIRE(t.at("sleep 5 ms") >= time2sleep);
}

TEST_CASE("Timer::operator[]") {
    utilities::Timer t;
    REQUIRE_THROWS_AS(t["not a key"], std::out_of_range);
}

TEST_CASE("Printing a timer") {
    std::chrono::milliseconds time2sleep(5);
    utilities::Timer t;
    std::this_thread::sleep_for(time2sleep);
    t.record("sleep 5 ms");
    std::stringstream ss;
    ss << t << std::endl;
    auto found = ss.str().find("sleep 5 ms : 0 h 0 m 0 s");
    REQUIRE(found != std::string::npos);
}
