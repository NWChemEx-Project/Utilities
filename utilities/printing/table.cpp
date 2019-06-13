#include "utilities/printing/tabl.hpp"
#include <vector>

using width_type

namespace utilities::printing {
namespace detail_ {

class TablePIMPL {
public:
    TablePIMPL(width_type max_width) : m_max_w_(max_width) {}

    //Returns the string representation of the table
    std::string str() const;

    width_type nrows() const { return m_data.size(); }

    width_type ncols() const { return nrows() == 0 ? 0 : m_data_[0].size(); }

    void add_rows(width_type n = 1) {
        m_data.resize(m_data.size() + n, std::vector<std::string>(ncols()));
    }

    void add_cols(width_type n = 1) {
        for(auto& x : m_data_) x.resize(x.size() + n, "");
    }

private:
    ///computes the widths of the data in
    std::vector<width_type> widths_of_data(width_type col_i) const {
        std::vector<width_type> rv;

        for(const auto& x : m_data_)
            rv.push_back()
    }
    width_type m_max_w_;

    std::vector<width_type> m_widths_;
    std::vector<std::vector<std::string>> m_data_;
};

}

Table::Table() = default;

Table::~Table() = default;

} //namespace utilities::printing
