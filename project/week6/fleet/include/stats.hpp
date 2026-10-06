#pragma once
/**
 * @file stats.hpp
 * @brief Small numeric helpers, written once for every type.
 * @author Zeid Kootbally
 *
 * @details Both functions are templates, so the whole definition is here in
 * the header. A template's body must be visible wherever it is called, or the
 * linker reports "undefined reference" (L6, Templates Go in Headers).
 */
#include <concepts>
#include <vector>

/**
 * @brief Limits a value to a range.
 *
 * @tparam T Any type with @c < and @c >, such as @c int for a speed in
 *           percent or @c double for a battery reading.
 * @param value The value to limit.
 * @param low The smallest value allowed.
 * @param high The largest value allowed.
 * @return @p low if @p value is below it, @p high if above it, else @p value.
 */
template <typename T>
T clamp_value(T value, T low, T high) {
    if (value < low) { return low; }
    if (value > high) { return high; }
    return value;
}

/**
 * @brief Average of a list of values.
 *
 * @tparam T A floating-point type, so the division is not integer division.
 * @param values The values to average.
 * @return Their average, or 0 for an empty list.
 */
template <std::floating_point T>
T average_of(const std::vector<T>& values) {
    if (values.empty()) {
        return T{0};
    }
    T sum{0};
    for (const T& value : values) {
        sum += value;
    }
    return sum / static_cast<T>(values.size());
}
