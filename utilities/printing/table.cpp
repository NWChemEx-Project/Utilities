#include "utilities/printing/tabl.hpp"
#include <vector>

using width_type  = typename Table::width_type;
using size_vector = std::vector<width_type>;
using row_type = std::vector<std::string>;
using table_type  = std::vector<row_type>;

namespace utilities::printing {
namespace detail_ {

class TablePIMPL {
public:
    TablePIMPL(width_type max_width) : m_max_w_(max_width) {}

    //Returns the string representation of the table
    std::string str() const;

    width_type nrows() const { return m_data.size(); }

    width_type ncols() const { return nrows() == 0 ? 0 : m_data_[0].size(); }

    void set_size(with_type m, width_type n) {
        m_data_.resize(m, row_type(n));
        m_widths_.resize(n)
    }

    void add_rows(width_type n = 1) {
        m_data.resize(nrows() + n, row_type(ncols()));
    }

    void add_cols(width_type n = 1) {
        for(auto& x : m_data_) x.resize(x.size() + n, "");
        m_widths_.resize(m_widths_.size() + n, 0);
    }

private:
    //computes the width of column i
    width_type width_of_col_(width_type col_i) const;

    width_type m_max_w_;

    //the widths that the user set
    size_vector m_widths_;

    table_type m_data_;
};

}

//---------------------------Implementations------------------------------------

inline size_vector TablePIMPL::widths_of_col_(width_type col_i) const {
    if(m_widths_[col_i] > 0) //Set by user
        return m_widths_[col_i];

    for(std::size_t row_i = 0; row_i < nrows(); ++row_i) {

    }
}


Table::Table() = default;

Table::~Table() = default;

} //namespace utilities::printing
