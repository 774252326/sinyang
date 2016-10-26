#pragma once

template <typename T>
T round(T x)
{
	return x < 0 ? ceil(x - 0.5) : floor(x + 0.5);
}