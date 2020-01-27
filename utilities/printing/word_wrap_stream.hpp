#pragma once
#include <memory>
#include <ostream>

namespace utilities::printing {
namespace detail_ {
class WordWrapBuffer;
}

/** @brief An std::ostream that automatically wraps its contents if they
 *         exceed the specified width.
 *
 *  WordWrapStream instances wrap another `std::ostream` instance. The resulting
 *  WordWrapStram instance prints to the wrapped instance and behaves exactly
 *  like the wrapped instance except that internally the WordWrapStream tracks
 *  how many characters have been printed and will automatically insert a line
 *  break when printing a word would cause the current line to have more
 *  characters than a predefined width. The word wrap only occurs if the user
 *  does not provide a newline character prior to a line reaching the predefined
 *  width. If a newline character is encountered before that, the internal
 *  counter is reset. Hence this class will preserve pre-existing formatting.
 */
class WordWrapStream : public std::ostream {
public:
    /// The type used to specify the predefined width
    using width_type = unsigned long;

    /** @brief Bestows an existing ostream with word wrapping abalities
     *
     * @param[in] os The address of the ostream instance we are wrapping. The
     *               lifetime of the ostream is not managed by the resulting
     *               WordWrapStream and it is the user's responsibility to
     *               ensure @p os stays in scope.
     * @param[in] w  The predefined width at which word wrapping occurs. Default
     *               is 80 characters.
     *
     * @throw std::bad_alloc if there is insufficient memory to make the PIMPL
     *                       instance. Strong throw guarantee.
     */
    explicit WordWrapStream(std::ostream* os, width_type w = 80);

    /** @brief Default dtor
     *
     *  This dtor does not do anything special to the wrapped ostream. Notably
     *  this means that it does not flush it. Since the ostream's lifetime must
     *  exceed the WordWrapStream's lifetime, the user is free to flush the
     *  stream after this dtor is called.
     *
     *  @throw none No throw guarantee.
     */
    ~WordWrapStream() noexcept override;

private:
    /// The instance that actually implements the class
    std::unique_ptr<detail_::WordWrapBuffer> m_pimpl_;
};

} // namespace utilities::printing
