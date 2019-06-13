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

    /// A reference to an entry in the table
    using reference = value_type&;

    /// A const reference to an entry in the table
    using const_reference = const value_type&;

    /// Type used to hold a row of the table
    using row_type = std::vector<value_type>;

    /// Type used to hold the table
    using table_type = std::vector<row_type>;

    /// Type of a vector of sizes
    using size_vec = std::vector<width_type>;

    explicit TablePIMPL(width_type max_width) noexcept : m_max_w_(max_width) {}

    TablePIMPL(width_type rows, width_type cols, width_type max_width);

    TablePIMPL(table_type data, width_type max_width) noexcept;

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

    /** @brief Returns the number of columns in the table
     *
     *  This function returns the number of columns as set by `add_cols` and
     *  `set_size`. It does not assert that those columns have data in them.
     *
     *  @return The number of columns in the table.
     *
     *  @throw none No throw guarantee.
     */
    width_type ncols() const noexcept;

    /** @brief Resizes the table so that it is the specified size.
     *
     *  This call will overwrite all data already entered into the table.
     *  Formatting options for
     *
     * @param[in] rows The number of rows the resulting table should have.
     * @param[in] cols The number of columns the resulting table should have.
     *
     * @throw std::bad_alloc if there is insufficient memory to allocate the
     *                       new table. Weak throw guarantee.
     */
    void set_size(width_type rows, width_type cols);

    void set_width(width_type col, width_type width) noexcept;

    void add_rows(width_type n = 1);

    /** @brief Adds a column to the table.
     *
     * @note Since the table is stored in row-major format, adding a column
     *       entails expanding each row. Hence if there is no rows we can't add
     * a column. This function will do nothing if there are no rows.
     *
     * @param[in] n The number of columns to add to each row. Defaults to 1.
     *
     * @throw std::bad_alloc if there is insufficient memory to grow the table.
     *                       Weak throw guarantee.
     */
    void add_cols(width_type n = 1);

    /** @brief Computes the width of a specified column.
     *
     *  When printing the table we need to know how many characters a column
     *  takes up. This function first sees if the user fixed the width of the
     *  column. If so we return that. If not we then loop over rows in that
     *  column and determine what the maximum length of a string in that
     *  column is. The latter computation takes into account values that contain
     *  newlines only counting the length of the largest line in the value.
     *
     * @param[in] col_i The column whose width we want to know.
     * @return The size of column in characters.
     */
    width_type width_of_col(width_type col_i) const;

    std::vector<width_type> space_out_cols() const;

    width_type height_of_row(width_type row_i) const;

    reference operator()(width_type row, width_type col);

    const_reference operator()(width_type row, width_type col) const;

private:
    width_type m_max_w_;

    table_type m_data_;

    // the widths that the user set
    std::vector<width_type> m_widths_;

    std::string m_lborder_ = "| ";
    std::string m_mborder_ = " | ";
    std::string m_rborder_ = " |";
    char m_under_          = '-';
    char m_intersect_      = '+';
};

//---------------------------Implementations------------------------------------

inline TablePIMPL::TablePIMPL(width_type rows, width_type cols,
                              width_type max_width) :
  TablePIMPL(max_width) {
    set_size(rows, cols);
}

inline TablePIMPL::TablePIMPL(table_type data, width_type max_width) noexcept :
  m_max_w_(max_width),
  m_data_(std::move(data)),
  m_widths_(m_data_.empty() ? 0 : m_data_[0].size()) {}

inline typename TablePIMPL::width_type TablePIMPL::ncols() const noexcept {
    return nrows() == 0 ? 0 : m_data_[0].size();
}

inline void TablePIMPL::set_size(width_type rows, width_type cols) {
    table_type(rows, row_type(cols)).swap(m_data_);
    m_widths_.resize(cols);
}

inline void TablePIMPL::set_width(width_type col, width_type width) noexcept {
    m_widths_[col] = width;
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
    }
    return w;
}

inline typename TablePIMPL::size_vec TablePIMPL::space_out_cols() const {
    const auto n = ncols();
    if(n == 0) return size_vec{};

    // How many characters do we loose to borders?
    const auto nborders =
      m_lborder_.size() + (n - 1) * m_mborder_.size() + m_rborder_.size();

    // How many characters did the user fix?
    auto user_chars = std::accumulate(m_widths_.begin(), m_widths_.end(), 0);

    // See if we have enough characters to honor user's requests
    if(user_chars + nborders > m_max_w_)
        throw std::runtime_error("Not enough char's to meet user's request");

    // For each column we bare minimal need to print one char
    if(nborders + n > m_max_w_)
        throw std::runtime_error("Not enough char's to print your table");

    // We now know the message is printable

    // Which columns do we need to figure out sizes for?
    size_vec auto_cols;
    for(width_type i = 0; i < n; ++i)
        if(m_widths_[i] == 0) auto_cols.push_back(i);

    // User specified all of them and they're printable
    if(auto_cols.empty()) return m_widths_;

    // How many chars do we have left to work with
    auto free_chars = m_max_w_ - user_chars - nborders;

    // Our plan is to give each of the remaining columns their width in char's
    // If that doesn't work we just give each column the same number of char's

    size_vec rv(m_widths_); // Our return value

    // How many char's are the column's widths?
    auto nrequest = 0;
    for(auto x : auto_cols) nrequest += width_of_col(x);

    // What's the average number of characters?
    auto extra_chars    = free_chars % n;
    auto avg_free_chars = (free_chars - extra_chars) / n;

    // Can we just give them their widths?
    bool is_good = nrequest <= free_chars;

    for(auto x : auto_cols) rv[x] = is_good ? width_of_col(x) : avg_free_chars;

    return rv;
}

inline typename TablePIMPL::width_type TablePIMPL::height_of_row(
  width_type row_i) const {}

inline typename TablePIMPL::reference TablePIMPL::operator()(width_type row,
                                                             width_type col) {
    return m_data_[row][col];
}

inline typename TablePIMPL::const_reference TablePIMPL::operator()(
  width_type row, width_type col) const {
    return m_data_[row][col];
}

} // namespace utilities::printing::detail_
