#pragma once

namespace utilities::detail_ {

template<typename ElementType>
class MathSetPIMPLItr
  : public RandomAccessIteratorBase<MathSetPIMPLItr<ElementType>, ElementType> {
private:
    using my_type = MathSetPIMPLItr<ElementType>;

public:
    ElementType& dereference() override { return (*m_parent_)[m_counter_]; }

    bool are_equal(const my_type& rhs) const noexcept override {
        return m_parent_ == rhs.m_parent_ && m_counter_ == rhs.m_counter_;
    }

    my_type& increment() override {
        ++m_counter_;
        return *this;
    }

    my_type& de

      private : size_type m_counter_;
    MathSetPIMPL<ElementType>* m_parent_;
};

template<typename ElementType>
class MathSetPIMPL {
public:
    reference operator[](size_type i) { return get_(i); }
    const_reference operator[](size_type i) const { return get_(i); }

private:
    virtual reference get_(size_type i)             = 0;
    virtual const_reference get_(size_type i) const = 0;
};

} // namespace utilities::detail_
