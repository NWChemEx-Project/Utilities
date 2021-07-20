#include "utilities/printing/word_wrap_stream.hpp"
#include "utilities/strings/string_tools.hpp"
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

namespace utilities::printing {

// Get the width type from WordWrapStream
using width_type = typename WordWrapStream::width_type;

namespace detail_ {

// Define the PIMPL class
class WordWrapBuffer : public std::streambuf {
public:
    // Takes the ostream we're wrapping and the width
    WordWrapBuffer(std::ostream* os, width_type w) noexcept :
      m_os_(os), m_w_(w) {}

protected:
    // This function does the heavy lifting
    std::streamsize xsputn(const char* s, std::streamsize n) override {
        using itr_type = std::istream_iterator<std::string>;
        std::istringstream iss(std::string(s, s + n));

        // Break paragraph into sentences
        auto sentences = strings::split_string(std::string(s, s + n), "\n");

	// Tracks if currently in a table.
	// A table starts when a line starts with a '+' character and ends when
	// an empty line is encountered.
	bool in_table = false;
	
        const auto nsentences = sentences.size();
        for(std::size_t si = 0; si < nsentences; ++si) { // Loop over sentences
            auto sentence = sentences[si];

	    // Detect start of a table when line starts with '+' character
	    if (sentence[0] == '+') {
		in_table = true;
	    }
	    // End of table if an empty line is encountered
	    else if (in_table && sentence.empty()) {
		in_table = false;
	    }
	    
            while(!sentence.empty()) {
                const unsigned short size = sentence.size();
		    
		if(m_nchars_ + size <= m_w_) { // Whole thing fits
                    (*m_os_) << sentence;
                    m_nchars_ += size;
                    break;
                }

                // Remainder of this while figures out where to break the line

                // How many characters do we have left?
                const auto char_left = m_w_ - m_nchars_;

                // Find the first space we can break on
                std::size_t offset = sentence.find(' ', 0);

                // Was there any spaces left?
                const bool no_spaces_left = (offset == std::string::npos);

                // Aren't on empty line and can't print any part of what remains
                if((no_spaces_left || offset > char_left) && m_nchars_ > 0) {
                    (*m_os_) << std::endl;
                    m_nchars_ = 0;
                    continue;
                }

		// Output the entire line if the remaining characters cannot fit
		// in an empty line, or we are currently in a table and ignoring
		// the character limit
                if(no_spaces_left || in_table) {
                    (*m_os_) << sentence;
                    break;
                }

                // See if we can fit more on the line
                while(offset != std::string::npos) {
                    auto temp = sentence.find(' ', offset + 1);
                    if(temp > char_left || temp == std::string::npos) break;
                    offset = temp;
                }

                (*m_os_) << sentence.substr(0, offset);
                (*m_os_) << std::endl;
                m_nchars_ = 0;
                sentence  = sentence.substr(offset + 1); // swallow the space
            }
            // Print a newline if we have more lines left
            if(si < nsentences - 1) {
                (*m_os_) << std::endl;
                m_nchars_ = 0;
            }

        } // loop over lines
        return n;
    }

    int overflow(int_type c) override {
        if(m_nchars_ == m_w_) {
            (*m_os_) << std::endl;
            m_nchars_ = 0;
        }
        (*m_os_) << traits_type::to_char_type(c);
        ++m_nchars_;

        return c;
    }

private:
    /// Where we write data to if an overflow occurs
    std::ostream* m_os_;

    /// The width of the page
    width_type m_w_;

    /// How many characters we've written
    width_type m_nchars_ = 0;
}; // class WordWrapBuffer

} // namespace detail_

//--------------------WordWrapStream Definitions--------------------------------

WordWrapStream::WordWrapStream(std::ostream* os, width_type w) :
  m_pimpl_(std::make_unique<detail_::WordWrapBuffer>(os, w)) {
    rdbuf(m_pimpl_.get());
}

WordWrapStream::~WordWrapStream() noexcept = default;

} // namespace utilities::printing
