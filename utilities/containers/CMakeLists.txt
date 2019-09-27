add_subdirectory(math_set)
list(APPEND headers containers/case_insensitive_map.hpp
                    containers/indexable_container_base.hpp
                    containers/own_or_borrow.hpp
)
set(UTILITIES_HEADERS "${headers}" PARENT_SCOPE)
