#pragma once
#include<ostream>
#include<memory>

namespace utilities::printing {
namespace detail_{ class WordWrapBuffer;}

/** @brief
 *
 */
class WordWrapStream : public std::ostream {
public:
    using width_type = unsigned long;

    explicit WordWrapStream(std::ostream* os, width_type w = 80);
    ~WordWrapStream() noexcept;
private:
    std::unique_ptr<detail_::WordWrapBuffer> m_pimpl_;
};

} //End namespace
