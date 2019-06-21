#include <array>
#include <catch2/catch.hpp>
#include <deque>
#include <forward_list>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utilities/printing/print_stl.hpp>
#include <utility>
#include <vector>

using namespace utilities::printing;

TEST_CASE("std::ostream<<(std::array)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::array<double, 0> a{};
        ss << a;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::array a{1.23, 2.34, 3.45};
        ss << a;
        std::stringstream corr;
        corr << '[' << a[0] << ", " << a[1] << ", " << a[2] << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::deque)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::deque<double> d;
        ss << d;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::deque d{1.23, 2.34, 3.45};
        ss << d;
        std::stringstream corr;
        corr << '[' << d[0] << ", " << d[1] << ", " << d[2] << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::forward_list)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::forward_list<double> d;
        ss << d;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::forward_list d{1.23, 2.34, 3.45};
        ss << d;
        std::stringstream corr;
        corr << '[' << 1.23 << ", " << 2.34 << ", " << 3.45 << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

// TEST_CASE("std::ostream<<(std::list)") {
//  std::stringstream ss;
//  SECTION("Empty") {
//    std::list<double> d;
//    ss << d;
//    REQUIRE(ss.str() == "[]");
//  }
//  SECTION("Non-Empty"){
//    std::list d{1.23, 2.34, 3.45};
//    ss << d;
//    std::stringstream corr;
//    corr << '[' << 1.23 << ", " << 2.34 << ", " << 3.45 << ']';
//    REQUIRE(ss.str() == corr.str());
//  }
//}

TEST_CASE("std::ostream<<(std::map)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::map<std::string, double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::map<std::string, double> d{{"Element 1", 1.23},
                                        {"Element 2", 2.34}};
        ss << d;
        std::stringstream corr;
        corr << "{(Element 1 : " << 1.23 << "), (Element 2 : " << 2.34 << ")}";
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::multimap)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::multimap<std::string, double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::multimap<std::string, double> d{{"Element 1", 1.23},
                                             {"Element 2", 2.34}};
        ss << d;
        std::stringstream corr;
        corr << "{(Element 1 : " << 1.23 << "), (Element 2 : " << 2.34 << ")}";
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::multiset)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::multiset<double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::multiset<double> q{1.23, 2.34, 3.45};
        ss << q;
        std::stringstream corr;
        corr << '{' << 1.23 << ", " << 2.34 << ", " << 3.45 << '}';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::optional") {
    std::stringstream ss;
    SECTION("No value") {
        std::optional<double> d;
        ss << d;
        REQUIRE(ss.str() == "nullopt");
    }
    SECTION("A value") {
        std::optional<int> d{1};
        ss << d;
        REQUIRE(ss.str() == "1");
    }
}

TEST_CASE("std::ostream<<(std::pair)") {
    std::pair p{"Hi", 1};
    std::stringstream ss;
    ss << p;
    REQUIRE(ss.str() == "(Hi, 1)");
}

TEST_CASE("std::ostream<<(std::priority_queue)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::priority_queue<double> d;
        ss << d;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::priority_queue<double> q;
        q.push(1.23);
        q.push(2.34);
        q.push(3.45);
        ss << q;
        std::stringstream corr;
        corr << '[' << 3.45 << ", " << 2.34 << ", " << 1.23 << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::queue)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::queue<double> d;
        ss << d;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::queue<double> q;
        q.push(1.23);
        q.push(2.34);
        q.push(3.45);
        ss << q;
        std::stringstream corr;
        corr << '[' << 1.23 << ", " << 2.34 << ", " << 3.45 << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::reference_wrapper") {
    int x{4};
    auto r = std::ref(x);
    std::stringstream ss;
    ss << r;
    REQUIRE(ss.str() == "4");
}

TEST_CASE("std::ostream<<(std::set)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::set<double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::set<double> q{1.23, 2.34, 3.45};
        ss << q;
        std::stringstream corr;
        corr << '{' << 1.23 << ", " << 2.34 << ", " << 3.45 << '}';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::stack)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::stack<double> d;
        ss << d;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::stack<double> q;
        q.push(1.23);
        q.push(2.34);
        q.push(3.45);
        ss << q;
        std::stringstream corr;
        corr << '[' << 3.45 << ", " << 2.34 << ", " << 1.23 << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::tuple)") {
    SECTION("Empty tuple") {
        std::tuple t{};
        std::stringstream ss;
        ss << t;
        REQUIRE(ss.str() == "()");
    }
    SECTION("Filled tuple") {
        std::tuple t{"Hi", 1, 3.14};
        std::stringstream ss;
        ss << t;
        REQUIRE(ss.str() == "(Hi, 1, 3.14)");
    }
}

TEST_CASE("std::ostream<<(std::unordered_map)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::unordered_map<std::string, double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::unordered_map<std::string, double> d{{"Element 1", 1.23},
                                                  {"Element 2", 2.34}};
        ss << d;
        std::stringstream corr;
        corr << "{(" << d.begin()->first << " : " << d.begin()->second << "), ("
             << (++d.begin())->first << " : " << (++d.begin())->second << ")}";
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::unordered_multimap)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::unordered_multimap<std::string, double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::unordered_multimap<std::string, double> d{{"Element 1", 1.23},
                                                       {"Element 2", 2.34}};
        ss << d;
        std::stringstream corr;
        corr << "{(" << d.begin()->first << " : " << d.begin()->second << "), ("
             << (++d.begin())->first << " : " << (++d.begin())->second << ")}";
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::unordered_multiset)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::unordered_multiset<double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::unordered_multiset<double> q{1.23, 2.34, 3.45};
        ss << q;
        std::stringstream corr;
        corr << '{' << *q.begin() << ", " << *(++q.begin()) << ", "
             << *(++(++q.begin())) << '}';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::unordered_set)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::unordered_set<double> d;
        ss << d;
        REQUIRE(ss.str() == "{}");
    }
    SECTION("Non-Empty") {
        std::unordered_set<double> q{1.23, 2.34, 3.45};
        ss << q;
        std::stringstream corr;
        corr << '{' << *q.begin() << ", " << *(++q.begin()) << ", "
             << *(++(++q.begin())) << '}';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("std::ostream<<(std::vector)") {
    std::stringstream ss;
    SECTION("Empty") {
        std::vector<double> a{};
        ss << a;
        REQUIRE(ss.str() == "[]");
    }
    SECTION("Non-Empty") {
        std::vector a{1.23, 2.34, 3.45};
        ss << a;
        std::stringstream corr;
        corr << '[' << a[0] << ", " << a[1] << ", " << a[2] << ']';
        REQUIRE(ss.str() == corr.str());
    }
}

TEST_CASE("Nesting") {
    std::vector<std::map<int, double>> c{{{0, 1.1}, {1, 2.2}, {2, 3.3}},
                                         {{0, 4.4}, {1, 5.5}, {2, 6.6}}};
    std::stringstream ss;
    ss << c;
    auto corr =
      "[{(0 : 1.1), (1 : 2.2), (2 : 3.3)}, {(0 : 4.4), (1 : 5.5), (2 : 6.6)}]";
    REQUIRE(ss.str() == corr);
}

struct ANonPrintableClass {};

TEST_CASE("Container of non-printable objects") {
    std::vector<ANonPrintableClass> v(3);

    std::stringstream ss;
    ss << v;
    std::stringstream corr;
    const auto name = typeid(ANonPrintableClass).name();
    corr << "[<" << name << " " << &v[0] << ">, ";
    corr << "<" << name << " " << &v[1] << ">, ";
    corr << "<" << name << " " << &v[2] << ">]";
    REQUIRE(corr.str() == ss.str());
}
