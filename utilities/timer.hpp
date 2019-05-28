#pragma once
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

namespace utilities {

class Timer {
private:
    using clock_type = std::chrono::high_resolution_clock;
    using time_point = typename clock_type::time_point;
public:
    Timer() noexcept : m_started_(clock_type::now()) {}
    void record(std::string desc);
    template<typename Fxn, typename...Args>
    void time_it(std::string desc, Fxn&& fxn, Args&&...args);
    void reset() noexcept { m_started_ = clock_type::now(); }
private:
    friend std::ostream& operator<<(std::ostream&, const Timer&);
    void record_(std::string desc, time_point t1, time_point t2);
    time_point m_started_;
    std::vector<std::string> m_times_;

};

inline std::ostream& operator<<(std::ostream& os, const Timer& t){
    for(const auto& x : t.m_times_)
        os << x <<std::endl;
    return os;
}


//------------------------------Implementations--------------------------------


inline void Timer::record(std::string desc) {
    auto t2 = clock_type::now();
    record_(std::move(desc), m_started_, std::move(t2));
    reset();
}

template<typename Fxn, typename...Args>
void Timer::time_it(std::string desc, Fxn&& fxn, Args&&...args) {
    auto t1 = clock_type::now();
    fxn(std::forward<Args>(args)...);
    auto t2 = clock_type::now();
    record_(std::move(desc), t1, t2);
    reset();
}

inline void Timer::record_(std::string desc, time_point t1, time_point t2) {
    using namespace std::chrono;
    auto h  = duration_cast<hours>(t2 -t1);
    auto m  = duration_cast<minutes>(t2-t1) - h;
    auto s  = duration_cast<seconds>(t2-t1) - h - m;
    auto ms = duration_cast<milliseconds>(t2-t1) - h - m - s;
    desc+=" : ";
    desc+=std::to_string(h.count()) + " h ";
    desc+=std::to_string(m.count()) + " m ";
    desc+=std::to_string(s.count()) + " s ";
    desc+=std::to_string(ms.count()) + " ms";
    m_times_.emplace_back(std::move(desc));
}

} //namespace utilities
