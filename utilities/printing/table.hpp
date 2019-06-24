#pragma once
#include <memory>
#include <string>
#include <vector>

namespace utilities::printing {
namespace detail_ {
class TablePIMPL;
}

/** @brief Formats tabular data into a pretty string for printing purposes
 *
 *  The Table's state is separated in two. The first part is the data. This is
 *  the stuff that goes inside the cells of the table. The second part is the
 *  formatting options. These currently include:
 *
 *  - widths of the columns
 *
 *  When a user asks to print the Table instance the internal data is formatted
 *  according to the formatting options. At the moment the formatting options
 *  are primarily geared at printing reST tables, but there should be sufficient
 *  generality to the PIMPL that it is possible to tweak the class to print
 *  other types of tables.
 */
class Table {
public:
    /// The type of an index like quantity
    using size_type = unsigned short;

    /// The type used to store the data contained in a cell
    using value_type = std::string;

    /// The type of a cell's content in a read/write state
    using reference = value_type&;

    /// The type of a cell's content in a read-only state
    using const_reference = const value_type&;

    /// The type of the container storing the data
    using table_type = std::vector<std::vector<value_type>>;

    /** @brief Makes an empty table that will be at most the specified number of
     *         characters wide.
     *
     *  @param[in] max_width The maximum width, in number of characters, that
     *                       the table may use to print itself. Defaults to 80
     *                       characters.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        PIMPL. Strong throw guarantee.
     */
    explicit Table(size_type max_width = 80);

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
     *                       the table may use to print itself. Defaults to 80
     *                       characters.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        data or the PIMPL. Strong throw guarantee.
     */
    Table(size_type rows, size_type cols, size_type max_width = 80);

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
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        PIMPL. Strong throw guarantee.
     */
    explicit Table(table_type data, size_type max_width = 80);

    /// Default dtor
    ~Table();

    /** @brief Returns the table as a string.
     *
     *  When a user calls this function the current state of this table
     *  instance
     *  will be used to compute a string representation of the table. This is
     *  where all the heavy lifting of the table class occurs.
     *
     *  @return The table in a string format.
     *
     *
     *  @throw std::runtime_error if the user's manually specified column widths
     *                            are not possible. Strong throw guarantee.
     *  @throw std::runtime_error if any line does not fit within the specified
     *                                or computed column widths. Stront throw
     *                            guarantee.
     *
     *  @throw std::bad_alloc if there is insufficient memory to allocate the
     *                        temporary buffers or the return. Strong throw
     *                        guarantee.
     */
    std::string str() const;

    /** @brief Resizes the table so that it has the specified number of columns
     *         and rows.
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

    /** @brief Adds one or more rows to the table
     *
     *  This function will append @p nrows new rows on to the table. The data
     *  elements of the new rows will be set to the null string.
     *
     *  @param[in] nrows The number of rows to add to the table. Defaults to 1.
     *
     *  @throw std::bad_alloc if there is insufficient memory to add the rows.
     *                        Strong throw guarantee.
     */
    void add_rows(size_type nrows = 1);

    /** @brief Adds one or more columns to the table.
     *
     *  This function will append @p ncols columns to each row of the table.
     *  Note that since the table is stored in row-major format, adding a column
     *  can not be done unless there is at least one row in the table. If there
     *  are no rows in the table, this function does nothing.
     *
     * @param[in] ncols The number of columns to add to each row. Defaults to 1.
     *
     * @throw std::bad_alloc if there is insufficient memory to grow the table.
     *                       Strong throw guarantee.
     */
    void add_cols(size_type ncols = 1);

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

    /** @brief Returns the number of rows in the table.
     *
     *  This function returns the number of rows as set by `add_rows` and
     *  `set_size`. It does not assert that those rows have data in them.
     *
     *  @return The number of rows in the table.
     *
     *  @throw none No throw guarantee.
     */
    size_type nrows() const noexcept;

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
    /// The instance that actually implements the Table class
    std::unique_ptr<detail_::TablePIMPL> m_pimpl_;
};

/** @brief Allows a table object to be printed to an ostream.
 *
 *  @relates Table
 *
 *  This function simply calls `Table::str()` on the provided Table instance.
 *
 *  @param[in] os The stream to write the table to.
 *  @param[in] t The table to write to the stream.
 *
 *  @return @p os containing @p t.
 *
 *  @throw std::runtime_error if the user's manually specified column widths
 *                            are not possible. Strong throw guarantee.
 *  @throw std::runtime_error if any line does not fit within the specified
 *                                or computed column widths. Stront throw
 *                            guarantee.
 *
 *  @throw std::bad_alloc if there is insufficient memory to allocate the
 *                        temporary buffers or the return. Strong throw
 *                        guarantee.
 */
inline std::ostream& operator<<(std::ostream& os, const Table& t) {
    return os << t.str();
}

} // namespace utilities::printing
