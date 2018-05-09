#pragma once

/** @file ForEach.hpp
 *
 *  Includes C-Preprocessor Macros for iterating over a list of arguments and
 *  applying a function to them.  The contents of this header are borrowed from
 *  StackOverflow.
 *
 *  Of the various macros in here only the following are intended for use:
 *  - CALL_MACRO_X_FOR_EACH : Calls a macro for each argument passed to the list
 *  - CALL_MACRO_X_FOR_EACH1: Calls a macro with one bound argument for each
 *                            argument in the list
 *
 */

/**
 * @brief Returns the N-th argument of a list.
 *
 * By padding what's to the left and what's to the right of element N, this
 * macro can be made to arbitrarily select an element from a list.  To select
 * say the third argument of a list you simply put M-3 placeholders in front of
 * it where M is the maximum number written out here.
 */
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

/// Guts of the CALL_MACRO_X_FOR_EACH macro
///{
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)
///}

/// Guts of the CALL_MACRO_X_FOR_EACH1 macro
///{
#define _fe1_0(_call, x, ...)
#define _fe1_1(_call, x, y) _call(x, y)
#define _fe1_2(_call, x, y, ...) _call(x, y) _fe1_1(_call, x, __VA_ARGS__)
#define _fe1_3(_call, x, y, ...) _call(x, y) _fe1_2(_call, x, __VA_ARGS__)
#define _fe1_4(_call, x, y, ...) _call(x, y) _fe1_3(_call, x, __VA_ARGS__)
///}

/**
 * @brief Applies a macro to each argument in a list
 *
 * @param[in] x The macro to apply
 * @param[in] ... The list of arguments to apply @p x to
 *
 * Example usage:
 * @code
 * #define STRING(x) std::string x{#x};
 *
 * CALL_MACRO_X_FOR_EACH(STRING, a,b,c)
 *
 * //The result will be
 * std::string a{"a"}; std::string b{"b"}; std:string c{"c"};
 */
#define CALL_MACRO_X_FOR_EACH(x, ...)                                       \
    _GET_NTH_ARG("ignored", __VA_ARGS__, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0) \
    (x, __VA_ARGS__)

/**
 * @brief Applies a macro to each argument in a list binding the first argument
 *        to the macro
 *
 * @param[in] _call The macro to apply
 * @param[in] x The argument to bind
 * @param[in] ... The list of arguments to apply @p _call to
 *
 * Example usage:
 * @code
 * #define STRING(x, y) std::string y{#x};
 *
 * CALL_MACRO_X_FOR_EACH1(STRING, "NULL", a,b,c)
 *
 * //The result will be
 * std::string a{"NULL"}; std::string b{"NULL"}; std:string c{"NULL"};
 */
#define CALL_MACRO_X_FOR_EACH1(_call, x, ...)                            \
    _GET_NTH_ARG("ignored", __VA_ARGS__, _fe1_4, _fe1_3, _fe1_2, _fe1_1, \
                 _fe1_0)                                                 \
    (_call, x, __VA_ARGS__)
