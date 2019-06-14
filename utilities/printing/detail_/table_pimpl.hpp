#pragma once
#include "utilities/printing/word_wrap_stream.hpp"
#include "utilities/strings/string_tools.hpp"
#include <algorithm> //For max_element
#include <numeric>   //accumulate
#include <sstream>
#include <stdexcept> //For runtime_error
#include <string>
#include <vector>

namespace utilities::printing::detail_ {

/** @brief Computes the border that goes between rows of a table
 *
 *  The row spacer under a column looks like:
 *  ```
 *  <intersection_char><underline><underline>....<intersection_char>
 *  ```
 *
 *  This function will assemble it for a series of columns given their
 *  widths, the characters used for borders, and the character to use for
 *  underlining.
 *
 *  @param[in] widths A container containing the width of each column
 *  @param[in] borders The three borders in table. Respectively the strings
 *                     are used as the left-most border, middle borders, and
 *                     right-most border.
 *  @param[in] underline The character to use for underlining.
 *  @param[in] intersect The character to use when a border meets the underline.
 *
 *  @return  A string suitable for insertion between rows.
 *
 *  @throw std::bad_alloc if there is insufficient memory to create the string.
 *                        Strong throw guarantee.
 *  @throw std::runtime_error if @p borders[0] does not contain 0 or an odd
 *                            number of characters. Strong throw guarantee.
 */
template<typename ContainerType>
std::string row_spacer(const ContainerType& widths,
                       const std::array<std::string, 3>& borders,
                       char underline, char intersect);

/** @brief Converts a row into a formatted string.
 *
 *  This function takes the provided row data and prints it out. It
 *  automatically adds padding to meet the specified widths and will
 *  intersperse the borders as appropriate.
 *
 * @tparam ContainerType The type of the container holding the widths.
 * @tparam DataType The type of the container holding the data to print.
 *
 * @param[in] widths A number of columns long array where element i is how many
 *                  characters column i should take up. Widths should not
 *                  include character counts for borders.
 * @param[in] row_data A two index object such that element (i, j) is the j-th
 *                     line of column i. The data is expected to be
 *                     preformatted to fit within the allotted column size.
 * @param[in] borders A three element array such that the first and last
 *                    elements are the strings to use for the left and right
 *                    borders respectively. The middle element will be used for
 *                    all other borders.
 *
 * @return The row, as a string
 *
 * @throw std::bad_alloc if there is insufficient memory to create the string.
 *                       Strong throw guarantee.
 * @throw std::runtime_error if any line does not fit within the column width
 *                           specified. Stront throw guarantee.
 */
template<typename ContainerType, typename DataType>
inline std::string print_row(const ContainerType& widths,
                             const DataType& row_data,
                             const std::array<std::string, 3>& borders);

/** @brief Class that actually does all of the logic required for printing a
 *         table.
 *
 *  This class assumes that the user wants the table to be printed out ASCII art
 *  style. It's primarily target at reST, but could probably be adapted to be
 *  more flexible.
 *
 *  The basic strategy of the class is to let the user specify as much of the
 *  formatting as they want to. Next the dimensions of the table are worked out.
 *  We start with the column widths. If the user has set column widths we
 *  respect those, otherwise we take the length of the longest value to be the
 *  column width respecting line breaks (e.g., `"a string\nwith two lines"` is
 *  14 characters long and not 23). If the lengths computed in this manner do
 *  not fit within the maximum width of the table we simply give each column the
 *  same number of characters to work with.
 *
 *  With the column widths worked out we can figure out the heights of each row.
 *  For a given row we consider the data in it, in a word wrapped form. The
 *  maximum number of lines any element takes is then used as the height. Given
 *  the dimensions all that remains is to loop over the table, row by row
 *  printing the cell borders and the cell contents.
 *
 *  @note At present no checks are done to ensure we adhere to the user's
 *        specified height requests. We also have not exposed the ability to
 *        set the height to the user.
 */
class TablePIMPL {
public:
    /// The type used to hold widths
    using size_type = unsigned short;

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
    using size_vec = std::vector<size_type>;

    /** @brief Makes an empty table that will be at most the specified number of
     *         characters wide.
     *
     *  @param[in] max_width The maximum width, in number of characters, that
     *                       the table may use to print itself.
     *  @throw none No throw guarantee.
     */
    explicit TablePIMPL(size_type max_width) noexcept : m_max_w_(max_width) {}

    /** @brief Constructs a table of the specified size and maximum width
     *
     *  This ctor makes a table that has the specified dimensions and width-wise
     *  will not use more than the specified number of characters. The contents
     *  of all cells is blank after this ctor has been called and needs to be
     *  filled in.
     *
     *  @param[in] rows The number of rows the resulting table should have.
     *  @param[in] cols The number of columns the resulting table should have.
     *  @param[in] max_width The maximum width, in number of characters, that
     *                       the table may use to print itself.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        data. Strong throw guarantee.
     */
    TablePIMPL(size_type rows, size_type cols, size_type max_width);

    /** @brief Constructs a table populated with the specified data and subject
     *         to the provided maximum width
     *
     *  This ctor makes a table that has dimensions taken from the provided
     *  data. Width-wise it will not use more than the specified number of
     *  characters.
     *
     *  @param[in] data The data to populate the table with.
     *  @param[in] max_width The maximum width, in number of characters, that
     *                       the table may use to print itself.
     *
     *  @throw none No throw guarantee.
     */
    TablePIMPL(table_type data, size_type max_width) noexcept;

    /** @brief Returns the table as a string.
     *
     *  When a user calls this function the current state of this table instance
     *  will be used to compute a string representation of the table. This is
     *  where all the heavy lifting of the table class occurs.
     *
     *  @return The table in a string format.
     *
     *
     *  @throw std::runtime_error if the user's manually specified column widths
     *                            are not possible. Strong throw guarantee.
     *  @throw std::runtime_error if any line does not fit within the specified
     *                            or computed column widths. Strong throw
     *                            guarantee.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        temporary buffers or the return. Strong throw
     *                        guarantee.
     */
    std::string str() const;

    /** @brief Returns the number of rows in the table.
     *
     *  This function returns the number of rows as set by `add_rows` and
     *  `set_size`. It does not assert that those rows have data in them.
     *
     *  @return The number of rows in the table.
     *
     *  @throw none No throw guarantee.
     */
    size_type nrows() const noexcept { return m_data_.size(); }

    /** @brief Returns the number of columns in the table
     *
     *  This function returns the number of columns as set by `add_cols` and
     *  `set_size`. It does not assert that those columns have data in them.
     *
     *  @return The number of columns in the table.
     *
     *  @throw none No throw guarantee.
     */
    size_type ncols() const noexcept;

    /** @brief Resizes the table so that it is the specified size.
     *
     *  This call will overwrite all data already entered into the table. It
     *  also will reset any user-specified column widths/row heights.
     *
     * @param[in] rows The number of rows the resulting table should have.
     * @param[in] cols The number of columns the resulting table should have.
     *
     * @throw std::bad_alloc if there is insufficient memory to allocate the
     *                       new table. Strong throw guarantee.
     */
    void set_size(size_type rows, size_type cols);

    /** @brief Allows the user to fix the width of a particular column.
     *
     *  During the printing of the table one of the steps is to determine the
     *  width of each column. This function allows the user to manually specify
     *  the width that should be used for a particular column.
     *
     * @param[in] col The column whose width should be fixed. Must be in range
     *                [0, ncols()).
     * @param[in] width The width, in characters, to fix the column at.
     *
     * @throw std::out_of_range if @p col is not in the range [0, ncols()).
     *                          Strong throw guarantee.
     */
    void set_width(size_type col, size_type width);

    /// Not respected at the moment, but hook is here for the future
    void set_height(size_type row, size_type height);

    /** @brief Sets the left, middle, or right vertical border
     *
     * @param[in] comp Which component (left=0, middle=1, right=2) of the border
     *                 to set. Must be in range [0, 3).
     * @param[in] new_val The value to set the border to. The middle border must
     *                    be symmetric about the central character.
     *
     * @throw std::out_of_range if @p comp is not in the range [0, 3). Strong
     *                          throw guarantee.
     * @throw std::runtime_error if @p new_val is not symmetric about the
     *                           central character.
     */
    void set_border(size_type comp, std::string new_val);

    /** @brief Adds a row to the table
     *
     *  This function will append @p n new rows on to the table. The data
     *  elements of the new rows will be set to the null string.
     *
     *  @param[in] n The number of rows to add to the table.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the rows.
     *                        Strong throw guarantee.
     */
    void add_rows(size_type n = 1);

    /** @brief Adds one or more columns to the table.
     *
     *  This function will append @p n columns to each row of the table. Note
     *  that since the table is stored in row-major format, adding a column
     *  can not be done unless there is at least one row in the table. If there
     *  are no rows in the table, this function does nothing.
     *
     * @param[in] n The number of columns to add to each row. Defaults to 1.
     *
     * @throw std::bad_alloc if there is insufficient memory to grow the table.
     *                       Strong throw guarantee.
     */
    void add_cols(size_type n = 1);

    /** @brief Computes the width of a specified column.
     *
     *  When printing the table we need to know how many characters a column
     *  takes up. This function first sees if the user fixed the width of the
     *  column. If so we return that. If not we then loop over rows in that
     *  column and determine what the maximum length of a string in that
     *  column is. The latter computation takes into account values that contain
     *  newlines only counting the length of the largest line in the value.
     *
     * @param[in] col_i The column whose width we want to know. Must be in the
     *                  range [0, ncols()).
     *
     * @return The size of column in characters.
     *
     * @throw std::out_of_range if the requested column is not in the range
     *                          [0, ncols()). Strong throw guarantee.
     */
    size_type width_of_col(size_type col_i) const;

    /** @brief Computes the width for printing each column.
     *
     *  This function first checks to see if the user specified the width of
     *  the columns. If the widths were not specified then it will automatically
     *  compute the widths.
     *
     *  @throw std::runtime_error if the user's manually specified column widths
     *                            are not possible. Strong throw guarantee.
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        temporary buffers or the return. Strong throw
     *                        guarantee.
     */
    size_vec space_out_cols() const;

    size_type height_of_row(size_type row_i) const;

    /** @brief Returns a row that has split into lines.
     *
     *  This function will word wrap each cell in the requested row, based on
     *  the spacings computed by space_out_cols(). Each cell in the
     *  resulting row will then be split by line.
     *
     * @param[in] row_i The row to format.
     *
     * @return A two-dimensional array such that element (i, j) is the j-th
     *         line of cell @p row_i, column j.
     */
    table_type get_row_data(size_type row_i) const;

    /** @brief Provides read/write access to the data in a particular cell
     *
     * @param[in] row The row the data element is in. Must be in range
     *                [0, nrow()).
     * @param[in] col The column the data element is in. Must be in range
     *                [0, ncol()).
     *
     * @return The requested cell in a read/write state.
     *
     * @throw std::out_of_range if either @p row or @p col is not in their
     *                          respective range. Strong throw guarantee.
     */
    reference operator()(size_type row, size_type col);

    /** @brief Provides read-only access to the data in a particular cell
     *
     * @param[in] row The row the data element is in. Must be in range
     *                [0, nrow()).
     * @param[in] col The column the data element is in. Must be in range
     *                [0, ncol()).
     * @return The requested cell in a read-only state.
     *
     * @throw std::out_of_range if either @p row or @p col is not in their
     *                          respective range. Strong throw guarantee.
     */
    const_reference operator()(size_type row, size_type col) const;

private:
    /// The maximum width (including borders) the table may have
    size_type m_max_w_;

    /// The contents of the cells
    table_type m_data_;

    /// The widths that the user set
    size_vec m_widths_;

    /// The heights that the user set
    size_vec m_heights_;

    /// The vertical borders of the table
    std::array<std::string, 3> m_borders_ = {"| ", " | ", " |"};

    /// The character to use for a horizontal border (except under headers)
    char m_under_ = '-';

    /// The character to use for a horizontal border under a header
    char m_header_ = '=';

    /// Character used at the intersection of the vertical and horizontal
    /// borders
    char m_intersect_ = '+';
};

//---------------------------Implementations------------------------------------

template<typename ContainerType>
std::string row_spacer(const ContainerType& widths,
                       const std::array<std::string, 3>& borders,
                       char underline, char intersect) {
    const auto n_cols = widths.size();

    // Determine the termination string for the right-side of a column
    const auto npad = borders[1].size() - 1;
    if(npad % 2 != 0)
        throw std::runtime_error("Padding on middle border must be symmetric");

    const auto rhs = std::string(npad / 2, underline) + intersect +
                     std::string(npad / 2, underline);

    // Assemble the row spacer
    std::stringstream rv;

    // The 1st intersect and any extra characters to account for lborder_
    rv << intersect << std::string(borders[0].size() - 1, underline);

    // The underline of each column except the last
    for(auto i = 0; i < n_cols - 1; ++i) {
        rv << std::string(widths[i], underline) + rhs;
    }

    // The last column
    rv << std::string(widths[n_cols - 1], underline);
    rv << std::string(borders[2].size() - 1, underline) << intersect;

    return rv.str();
}

template<typename ContainerType, typename DataType>
std::string print_row(const ContainerType& widths, const DataType& row_data,
                      const std::array<std::string, 3>& borders) {
    std::stringstream ss;

    // Determine the height of this row
    auto l      = [](auto i, auto j) { return i.size() < j.size(); };
    auto height = std::max_element(row_data.begin(), row_data.end(), l)->size();

    const auto ncols = widths.size();

    for(auto line_i = 0; line_i < height; ++line_i) {
        ss << borders[0];
        for(auto col_i = 0; col_i < ncols; ++col_i) {
            const auto width_i = widths[col_i];

            if(row_data[col_i].size() <= line_i) { // No more lines, print blank
                ss << std::string(width_i, ' ');
            } else {
                const auto& line = row_data[col_i][line_i];

                if(line.size() > width_i)
                    throw std::runtime_error(
                      "Line is larger than column width");

                ss << line << std::string(width_i - line.size(), ' ');
            }
            ss << (col_i != ncols - 1 ? borders[1] : borders[2]);
        }
        ss << std::endl;
    }
    return ss.str();
}

inline TablePIMPL::TablePIMPL(size_type rows, size_type cols,
                              size_type max_width) :
  TablePIMPL(table_type(rows, row_type(cols)), max_width) {}

inline TablePIMPL::TablePIMPL(table_type data, size_type max_width) noexcept :
  m_max_w_(max_width),
  m_data_(std::move(data)),
  m_widths_(m_data_.empty() ? 0 : m_data_[0].size()),
  m_heights_(m_data_.empty() ? 0 : m_data_.size()) {}

inline std::string TablePIMPL::str() const {
    std::stringstream ss;

    if(nrows() == 0 || ncols() == 0) return ss.str();

    auto widths = space_out_cols();
    auto spacer = row_spacer(widths, m_borders_, m_under_, m_intersect_);

    ss << spacer << std::endl; // Top of table

    // Print out the header/first row of table
    ss << print_row(widths, get_row_data(0), m_borders_); // First line

    if(nrows() == 1) { // Don't print header spacer if we only have 1 row
        ss << spacer;
        return ss.str();
    }
    ss << row_spacer(widths, m_borders_, m_header_, m_intersect_) << std::endl;

    // Body of table
    for(auto row_i = 1; row_i < nrows() - 1; ++row_i) {
        ss << print_row(widths, get_row_data(row_i), m_borders_);
        ss << spacer << std::endl;
    }

    // Foot of table
    ss << print_row(widths, get_row_data(nrows() - 1), m_borders_);

    ss << spacer; // End of table

    return ss.str();
}

inline typename TablePIMPL::size_type TablePIMPL::ncols() const noexcept {
    return nrows() == 0 ? 0 : m_data_[0].size();
}

inline void TablePIMPL::set_size(size_type rows, size_type cols) {
    table_type temp_data(rows, row_type(cols));
    size_vec temp_widths(cols);
    size_vec temp_heights(rows);
    temp_data.swap(m_data_);
    temp_widths.swap(m_widths_);
    temp_heights.swap(m_heights_);
}

inline void TablePIMPL::set_width(size_type col, size_type width) {
    m_widths_.at(col) = width;
}

inline void TablePIMPL::set_height(size_type row, size_type height) {
    m_heights_.at(row) = height;
}

inline void TablePIMPL::set_border(size_type comp, std::string value) {
    if(comp == 1 && (value.size() % 2) != 1)
        throw std::runtime_error("The middle border must be symmetric");
    m_borders_.at(comp) = std::move(value);
}

inline void TablePIMPL::add_rows(size_type n) {
    const auto new_nrows = nrows() + n;
    m_data_.reserve(new_nrows);
    m_heights_.reserve(new_nrows);
    m_data_.resize(new_nrows, row_type(ncols()));
    m_heights_.resize(new_nrows);
}

inline void TablePIMPL::add_cols(size_type n) {
    const auto new_ncols = ncols() + n;
    for(auto& x : m_data_) x.reserve(new_ncols);
    m_widths_.reserve(new_ncols);

    for(auto& x : m_data_) x.resize(new_ncols);
    m_widths_.resize(new_ncols);
}

inline typename TablePIMPL::size_type TablePIMPL::width_of_col(
  size_type col_i) const {
    if(m_widths_.at(col_i) > 0) // Set by user
        return m_widths_[col_i];

    size_type w = 0;
    for(std::size_t row_i = 0; row_i < nrows(); ++row_i) {
        // Split contents into sentences
        auto sents     = strings::split_string(m_data_[row_i][col_i], "\n");
        auto l         = [](auto i, auto j) { return i.size() < j.size(); };
        auto temp_itr  = std::max_element(sents.begin(), sents.end(), l);
        size_type temp = temp_itr != sents.end() ? temp_itr->size() : 0ul;
        w              = std::max(temp, w);
    }
    return w;
}

inline typename TablePIMPL::size_vec TablePIMPL::space_out_cols() const {
    const auto n = ncols();
    if(n == 0) return size_vec{};

    // How many characters do we loose to borders?
    const auto nborders = m_borders_[0].size() +
                          (n - 1) * m_borders_[1].size() + m_borders_[2].size();

    // How many characters did the user fix?
    auto user_chars = std::accumulate(m_widths_.begin(), m_widths_.end(), 0);

    // See if we have enough characters to honor user's requests
    if(user_chars + nborders > m_max_w_)
        throw std::runtime_error("Not enough char's to meet user's request");

    // We assume the message is printable, i.e., we can at least do one word per
    // line

    // Which columns do we need to figure out sizes for?
    size_vec auto_cols;
    for(size_type i = 0; i < n; ++i)
        if(m_widths_[i] == 0) auto_cols.push_back(i);

    // User specified all of them and they're printable, i.e., we're done
    if(auto_cols.empty()) return m_widths_;

    // How many chars do we have left to work with
    const auto free_chars = m_max_w_ - user_chars - nborders;

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

inline typename TablePIMPL::size_type TablePIMPL::height_of_row(
  size_type row_i) const {
    if(row_i >= nrows()) throw std::out_of_range("Requested row is >= nrows()");

    if(m_heights_[row_i] > 0) return m_heights_[row_i];

    auto widths   = space_out_cols();
    auto row_data = get_row_data(row_i);
    auto l        = [](auto i, auto j) { return i.size() < j.size(); };
    return std::max_element(row_data.begin(), row_data.end(), l)->size();
}

inline typename TablePIMPL::table_type TablePIMPL::get_row_data(
  size_type row_i) const {
    auto widths = space_out_cols();
    table_type rv(ncols());
    for(size_type col_i = 0; col_i < ncols(); ++col_i) {
        // Word wrap the cell, using width of cell
        std::stringstream ss;
        WordWrapStream s(&ss, widths[col_i]);
        s << (*this)(row_i, col_i);
        rv[col_i] = strings::split_string(ss.str(), "\n");
    }
    return rv;
}

inline typename TablePIMPL::reference TablePIMPL::operator()(size_type row,
                                                             size_type col) {
    return m_data_.at(row).at(col);
}

inline typename TablePIMPL::const_reference TablePIMPL::operator()(
  size_type row, size_type col) const {
    return m_data_.at(row).at(col);
}

} // namespace utilities::printing::detail_
