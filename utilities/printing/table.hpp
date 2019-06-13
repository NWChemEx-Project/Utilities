#pragma once
#include <memory>

namespace utilities::printing {
namespace detail_ {
class TablePIMPL;
}

class Table {
public:
    using width_type = unsigned short;

    explicit Table(width_type max_width = 80);

    ~Table();

private:
    /// The instance that actually implements the Table class
    std::unique_ptr<detail_::TablePIMPL> m_pimpl_;
};

} // namespace utilities::printing
