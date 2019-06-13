#pragma once
#include "utilities/strings/string_tools.hpp"
#include <algorithm> //For max_element
#include <string>
#include <vector>

namespace utilities::printing::detail_ {

class TablePIMPL {
public:
    /// The type used to hold widths
    using width_type = unsigned short;

    /// The type used to store a table entry
    using value_type = std::string;

    /// Type used to hold a row of the table
    using row_type = std::vector<value_type>;

    /// Type used to hold the table
    using table_type = std::vector<row_type>;

    explicit TablePIMPL(width_type max_width) noexcept : m_max_w_(max_width) {}

    // Returns the string representation of the table
    // std::string str() const;

    /** @brief Returns the number of rows in the table.
     *
     *  This function returns the number of rows as set by `add_rows` and
     *  `set_size`. It does not assert that those rows have data in them.
     *
     *  @return The number of rows in the table.
     *
     *  @throw none No throw guarantee.
     */
    width_type nrows() const noexcept { return m_data_.size(); }

    width_type ncols() const { return nrows() == 0 ? 0 : m_data_[0].size(); }

    void set_size(width_type m, width_type n);

    void add_rows(width_type n = 1);

    /** @brief Adds a column to the table.
     *
     * @note Since the table is stored in row-major format, adding a column
     *       entails expanding each row. Hence if there is no rows we can't add
     * a column. This function will do nothing if there are no rows.
     *
     * @param[in] n The number of columns to add to each row. Defaults to 1.
     *
     * @throw std::bad_alloc if there is insufficent memory to grow the table.
     *                       Strong throw guarantee.
     */
    void add_cols(width_type n = 1);

    // computes the width of column i
    width_type width_of_col(width_type col_i) const;

private:
    width_type m_max_w_;

    // the widths that the user set
    std::vector<width_type> m_widths_;

    table_type m_data_;
};

//---------------------------Implementations------------------------------------

inline void TablePIMPL::set_size(width_type m, width_type n) {
    m_data_.resize(m, row_type(n));
    m_widths_.resize(n);
}

inline void TablePIMPL::add_rows(width_type n) {
    m_data_.resize(nrows() + n, row_type(ncols()));
}

inline void TablePIMPL::add_cols(width_type n) {
    for(auto& x : m_data_) x.resize(x.size() + n, "");
    m_widths_.resize(m_widths_.size() + n, 0);
}

inline typename TablePIMPL::width_type TablePIMPL::width_of_col(
  width_type col_i) const {
    if(m_widths_[col_i] > 0) // Set by user
        return m_widths_[col_i];

    width_type w = 0;
    for(std::size_t row_i = 0; row_i < nrows(); ++row_i) {
        // Split contents into sentences
        auto sents = strings::split_string(m_data_[row_i][col_i], "\n");
        auto l     = [](auto i, auto j) { return i.size() < j.size(); };
        width_type temp =
          std::max_element(sents.begin(), sents.end(), l)->size();
        w = std::max(temp, w);
        ;
    }
    return w;
}

} // namespace utilities::printing::detail_
