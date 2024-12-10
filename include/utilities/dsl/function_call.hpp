/*
 * Copyright 2024 NWChemEx-Project
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

#pragma once
#include <type_traits>
#include <utilities/dsl/n_ary_op.hpp>

namespace utilities::dsl {

/** @brief Represents calling operator(Args...) on an object
 *
 *  @tparam Args The types of the arguments passed to operator(). Note that the
 *               first type in Args is the type of the object the implicit this
 *               pointer points to.
 *
 *  This class is essentially a strong type over top of NaryOp to signal
 *  that the N-ary operation is a function call (or at the least represented
 *  with by `operator()`).
 */
template<typename... Args>
class FunctionCall : public NAryOp<FunctionCall<Args...>, Args...> {
private:
    /// Type of *this
    using my_type = FunctionCall<Args...>;

    /// Type *this inherits from
    using op_type = NAryOp<my_type, Args...>;

public:
    /// Reuse the base class's ctor
    using op_type::op_type;
};

} // namespace utilities::dsl