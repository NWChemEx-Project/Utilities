#include "utilities/printing/detail_/table_pimpl.hpp"
#include "utilities/printing/table.hpp"
#include <vector>

namespace utilities::printing {

using width_type = typename Table::width_type;

Table::Table(width_type max_width) :
  m_pimpl_(std::make_unique<detail_::TablePIMPL>(max_width)) {}

Table::~Table() = default;

} // namespace utilities::printing
