#pragma once
#include "utilities/containers/math_set/detail_/math_set_pimpl.hpp"

namespace utilities::detail_ {

template<typename T>
class MathSetViewPIMPL : public MathSetPIMPL<T> {
private:
    using base_type = MathSetPIMPL<T>;

    /// Type used to alias an element of another set
    using view_type = std::reference_wrapper<T>;

public:
    /// Type of an element stored in the aliased set
    using value_type = typename base_type::value_type;

    /// Type of a read-only reference to an element of this set
    using const_reference = typename base_type::const_reference;

    /// The type used indexing and offsets, integral like
    using size_type = typename base_type::size_type;

    MathSetViewPIMPL() = default;

    explicit MathSetViewPIMPL(std::vector<view_type> data) :
      m_data_(std::move(data)) {}

private:
    const_reference get_(size_type i) const override {
        return m_data_[i].get();
    }

    size_type count_(const_reference elem) const noexcept override {
        auto l = [=](auto x) { return x.get() == elem; };
        return std::find_if(m_data_.begin(), m_data_.end(), l) != m_data_.end();
    }

    void push_back_(value_type elem) override {
        throw std::runtime_error(
          "Shouldn't be able to get here from public API");
    }

    size_type size_() const noexcept override { return m_data_.size(); }

    std::vector<view_type> m_data_;
};

template<typename T>
MathSetViewPIMPL(std::vector<std::reference_wrapper<T>>)->MathSetViewPIMPL<T>;

} // namespace utilities::detail_
