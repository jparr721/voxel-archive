#pragma once

#include <algorithm>
#include <functional>

namespace vx::util {
    template<typename Container>
    inline auto inContainer(const Container &container, const typename Container::value_type &value) -> int {
        for (int ii = 0; ii < container.size(); ++ii) {
            const auto &cv = container.at(ii);
            if (cv == value) { return ii; }
        }
        return -1;
    }

    template<typename Container, typename Value>
    inline auto inContainer(const Container &container, const Value &value,
                            const std::function<bool(const typename Container::value_type &, const Value &)> &fn)
            -> int {
        for (int ii = 0; ii < container.size(); ++ii) {
            const auto &cv = container.at(ii);
            if (fn(cv, value)) { return ii; }
        }
        return -1;
    }
}// namespace vx::util
