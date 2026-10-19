//
// Created by n08i40k on 31.01.2024.
//

#ifndef RANDOM_VALUE_H
#define RANDOM_VALUE_H

#include <random>

class random_value {
public:
	template<typename T,
		typename = std::enable_if_t<std::is_floating_point_v<T> >>
	static T generate(T min, T max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<T> distr(min, max);

		return distr(gen);
	}

	template<typename T,
		typename = std::enable_if_t<std::is_integral_v<T> >,
		typename = std::enable_if_t<!std::is_same_v<T, bool>>>
	static T generate(T min, T max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<T> distr(min, max);

		return distr(gen);
	}
};

#endif //RANDOM_VALUE_H
