#pragma once

template <typename T>
T clamp_value(T value, T low, T high);

// [Slide 40] Templates Go in Headers
// The declaration only. The body is in stats.cpp, which is the mistake.
