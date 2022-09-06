/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "utilities/printing/detail_/table_pimpl.hpp"
#include "utilities/printing/table.hpp"
#include <vector>

namespace utilities::printing {

using size_type       = typename Table::size_type;
using reference       = typename Table::reference;
using const_reference = typename Table::const_reference;
using table_type      = typename Table::table_type;

Table::Table(size_type max_width) :
  m_pimpl_(std::make_unique<detail_::TablePIMPL>(max_width)) {}

Table::Table(size_type rows, size_type cols, size_type max_width) :
  m_pimpl_(std::make_unique<detail_::TablePIMPL>(rows, cols, max_width)) {}

Table::Table(table_type data, size_type max_width) :
  m_pimpl_(std::make_unique<detail_::TablePIMPL>(std::move(data), max_width)) {}

Table::~Table() = default;

std::string Table::str() const { return m_pimpl_->str(); }

void Table::set_size(size_type rows, size_type cols) {
    m_pimpl_->set_size(rows, cols);
}

void Table::add_rows(size_type nrows) { m_pimpl_->add_rows(nrows); }

void Table::add_cols(size_type ncols) { m_pimpl_->add_cols(ncols); }

void Table::set_width(size_type col, size_type width) {
    m_pimpl_->set_width(col, width);
}

size_type Table::nrows() const noexcept { return m_pimpl_->nrows(); }

size_type Table::ncols() const noexcept { return m_pimpl_->ncols(); }

reference Table::operator()(size_type row, size_type col) {
    return (*m_pimpl_)(row, col);
}

const_reference Table::operator()(size_type row, size_type col) const {
    return (*m_pimpl_)(row, col);
}
} // namespace utilities::printing
