#pragma once
#include <utilities/dsl/detail_/leaf_holder.hpp>

namespace utilities::dsl {

class Leaf {
private:
    /// Base type of the holder
    using leaf_holder = detail_::LeafHolderBase;

    /// Type of a pointer to the holder
    using leaf_pointer = typename leaf_holder::leaf_pointer;

public:
    template<typename T>
    decltype(auto) value() {
        using holder_type = detail_::LeafHolder<T>;
        auto p            = dynamic_cast<holder_type*>(*m_holder_);
        if(p == nullptr) throw std::runtime_error("Type is not T");
        return p->value();
    }

    template<typename T>
    decltype(auto) value() const {
        using holder_type = detail_::LeafHolder<T>;
        auto p            = dynamic_cast<const holder_type*>(*m_holder_);
        if(p == nullptr) throw std::runtime_error("Type is not T");
        return p->value();
    }

private:
    leaf_pointer m_holder_;
};

template<typename T>
auto make_leaf(T value) {
    return Leaf(std::make_unique<detail_::TypedLeafHolder<T>>(value);
}

template<typename T>
auto unwrap_leaf(Leaf& leaf) {
    return leaf.template value<T>();
}

} // namespace utilities::dsl