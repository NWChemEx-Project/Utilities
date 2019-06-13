#include "utilities/printing/word_wrap_stream.hpp"
#include <sstream>
#include <iterator>
#include <vector>

namespace utilities::printing {

//Get the width type from WordWrapStream
using width_type = typename WordWrapStream::width_type;

namespace detail_ {

//Define the PIMPL class
class WordWrapBuffer : public std::streambuf {
public:
  //Takes the ostream we're wrapping and the width
  WordWrapBuffer(std::ostream* os, width_type w) noexcept :
  m_os_(os), m_w_(w) {}
protected:
//This function does the heavy lifting
std::streamsize xsputn(const char* s, std::streamsize n) override {
      using itr_type = std::istream_iterator<std::string>;
      std::istringstream iss(std::string(s,s + n));
      std::string sentence;
      std::vector<std::string> sentences;
      while(getline(iss, sentence)) {
        sentences.emplace_back(std::move(sentence));
      }

      const auto nsentences = sentences.size();
      for(std::size_t si = 0; si < nsentences; ++si){ //Loop over sentences
          //Break lines into words
          std::istringstream iss2(sentences[si]);
          std::vector<std::string> words;
          words.insert(words.end(), itr_type(iss2), itr_type());

          for(const auto& wordi : words) { //Loop over words
              const unsigned short size = wordi.size();
              //The new width (if there's characters we need a space
              const auto new_w = size + (m_nchars_ > 0 ? m_nchars_ + 1 : 0);
              const bool fits = new_w <= m_w_;

              //Add the space if we need it
              if(fits && m_nchars_ > 0) {
                  (*m_os_) << ' ';
                  ++m_nchars_;
              }
              else if(!fits) { //If it doesn't fit put it on next line
                  (*m_os_) << std::endl;
                  m_nchars_  = 0;
              }
              //else is it fits, but we don't need the space

              //Print the word
              (*m_os_) << wordi;
               m_nchars_ += size;
          } //loop over words

          //Print a newline if we have more lines left
          if(si < nsentences - 1) {
            (*m_os_) << std::endl;
            m_nchars_ = 0;
          }

      } //loop over lines
      return n;
}

private:
  ///Where we write data to if an overflow occurs
  std::ostream* m_os_;

  ///The width of the page
  width_type m_w_;

  ///How many characters we've written
  width_type m_nchars_ = 0;
}; //class WordWrapBuffer

} //namespace detail_

//--------------------WordWrapStream Definitions--------------------------------

WordWrapStream::WordWrapStream(std::ostream *os, width_type w) :
  m_pimpl_(std::make_unique<detail_::WordWrapBuffer>(os, w)) {
    rdbuf(m_pimpl_.get());
}

WordWrapStream::~WordWrapStream() noexcept = default;

} //namespace utilities::printing
