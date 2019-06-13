#pragma once

namespace utilities::printing {
namespace detail_ { class TablePIMPL; }

class Table {
public:
    using width_type = unsigned short;

    Table(width_type max_width = 80);
    ~Table();
private:
    std::unique_ptr<detail_::TablePIMPL> m_pimpl_;
};

} //namespace utilities::printing
