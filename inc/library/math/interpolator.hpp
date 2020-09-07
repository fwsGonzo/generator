#pragma once
#include <map>
#include <algorithm>

template <typename T>
class Interpolator {
public:
	using callback_t = std::function<T (const T&, const T&, float)>;

	bool good() const noexcept { return !m_points.empty(); }
	T get(float position, callback_t callback) const;

	template <typename... Args>
	auto try_emplace(Args&&... args) {
		return this->m_points.try_emplace(std::forward<Args> (args)...);
	}
private:
	std::map<float, T> m_points;
};

template <typename T>
inline T Interpolator<T>::get(float pos, callback_t callback) const
{
	auto it = m_points.lower_bound(pos);
	// check if we are at or past the end (NOTE: covers size == 1)
	if (it == m_points.end() || std::next(it) == m_points.end()) {
		const auto& rit = m_points.rbegin();
		return callback(rit->second, rit->second, 1.0f);
	}
	auto nxt = std::next(it);
	float lerp = (pos - it->first) / (nxt->first - it->first);
	return callback(it->second, nxt->second, lerp);
}
