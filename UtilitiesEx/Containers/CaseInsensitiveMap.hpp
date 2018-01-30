#pragma once
#include <map>
#include <string>

namespace UtilitiesEx {

template<typename value_type>
class CaseInsensitiveMap {
public:
    using key

    /**
     * @brief
     * @throw None. No throw guarantee.
     */
    CaseInsensitiveMap()noexcept = default;

    /**
     * @brief Makes a deep copy of another instance.
     * @param[in] rhs The instance to copy.
     * @throw std::bad_alloc if there is insufficient memory for the copy. Strong
     *        throw guarantee.
     */
    CaseInsensitiveMap(const CaseInsensitiveMap& /*rhs*/) = default;

    /**
     * @brief Takes ownership of another instance.
     * @param[in] rhs The instance to take ownership of.  After this call @p rhs
     *            is in a valid, but otherwise undefined state.
     * @throw None. No throw guarantee.
     */
    CaseInsensitiveMap(CaseInsensitiveMap&& /*rhs*/)noexcept = default;

    /**
     * @brief Cleans up any memory held by the current instance.
     * @throw None. No throw guarantee.
     *
     */
    ~CaseInsensitiveMap()noexcept = default;

    /**
     * @brief Assigns a deep copy of another instance's state to the current
     *        instance.
     * @param[in] rhs The instance to copy.
     * @return The current instance containing a deep-copy of @p rhs's state.
     * @throw std::bad_alloc if there is insufficient memory for the copy. Strong
     *        throw guarantee.
     */
    CaseInsensitiveMap& operator=(const CaseInsensitiveMap& /*rhs*/)= default;

    /**
     * @brief Takes ownership of another instance.
     * @param[in] rhs The instance to take ownership of.  After this call @p rhs
     *            is in a valid, but otherwise undefined state.
     * @return The current instance containing @p rhs's state.
     * @throw None. No throw guarantee.
     */
    CaseInsensitiveMap&
    operator=(CaseInsensitiveMap&& /*rhs*/)noexcept= default;

private:
    /**
     * @brief Implements a case insensitive less-than operation for two
     * std::strings.
     *
     * @note This is shamelessly stolen from StackOverflow's topic:
     * "Making mapfind operation case insensitive"
     */
    struct CaseInsensitiveLess_
    {
        // case-independent (ci) compare_less binary function
        struct nocase_compare
        {
            bool operator() (const unsigned char& c1, const unsigned char& c2) const {
                return tolower (c1) < tolower (c2);
            }
        };
        bool operator() (const std::string & s1, const std::string & s2) const {
            return std::lexicographical_compare
                    (s1.begin (), s1.end (),   // source range
                     s2.begin (), s2.end (),   // dest range
                     nocase_compare ());  // comparison
        }
    };

};

}
