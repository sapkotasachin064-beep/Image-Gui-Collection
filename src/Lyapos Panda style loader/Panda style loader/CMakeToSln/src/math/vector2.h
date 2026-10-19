//
// Created by n08i40k on 14.12.2023.
//

#ifndef ZENO_URPROJECT_REWRITE_VECTOR2_H
#define ZENO_URPROJECT_REWRITE_VECTOR2_H

#include <cstdint>
#include <stdexcept>
#include <utility>

template<typename ReqVT, class T>
concept vector2_like = requires(T vector) {
	{ vector.x } -> std::convertible_to<ReqVT>;
	{ vector.y } -> std::convertible_to<ReqVT>;
};

template<typename ValueT>
class vector2 {
public:
	ValueT x, y;

	vector2() : vector2(0, 0) {}

	template<typename NumberT>
	vector2(NumberT x,
	        NumberT y) {
		this->x = static_cast<ValueT>(x);
		this->y = static_cast<ValueT>(y);
	}

	template<typename NumberT>
	explicit
	vector2(vector2<NumberT> other_type) : vector2(
		                                     other_type.x,
		                                     other_type.y) {}

	template<class VectorT>
	explicit
	vector2(const VectorT& custom_vector) : vector2(
		                                      custom_vector.x,
		                                      custom_vector.y) {}

	constexpr ValueT&
	operator[](const uint8_t index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		default: return 0;
		}
	}

	constexpr ValueT
	operator[](const uint8_t index) const { return operator[](index); }

	template<class T>
		requires vector2_like<ValueT, T>
	vector2
	rotate(const T center,
	       const double degrees) const {
		// я не математик.
		// https://www.itcodar.com/c-plus-1/c-rotating-a-vector-around-a-certain-point.html

		double radians = degrees * (M_PI / 180);
		double cos_theta = cos(radians);
		double sin_theta = sin(radians);

		return {
			(cos_theta * (x - center.x) - sin_theta * (y - center.y) + center.x),
			(sin_theta * (x - center.x) + cos_theta * (y - center.y) + center.y)
		};
	}

	void
	swap() {
		ValueT saved_y = y;
		y = x;
		x = saved_y;
	}

	[[nodiscard]] bool
	is_zero() const { return x == 0 && y == 0; }

	[[nodiscard]] double
	magnitude() const { return sqrt(x * x + y * y); }

	void
	normalize() {
		if (const long double sum = x * x + y * y;
			sum > 0) {
			long double d = 1.0 / sqrtl(sum);
			x = d * x;
			y = d * y;
		}
		else
			x = 1.0;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2
	bound(VectorT min,
	      VectorT max) const {
		return vector2{
			std::max<ValueT, ValueT>(min.x, std::min<ValueT, ValueT>(max.x, x)),
			std::max<ValueT, ValueT>(min.y, std::min<ValueT, ValueT>(max.y, y))
		};
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	ValueT
	cross(
		const VectorT& b) const { return x * b.y - y * b.x; }

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	VectorT
	convert() const { return VectorT{x, y}; }

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2<NumberT>
	convert() const { return {x, y}; }

	template<class VectorT>
		requires (std::is_class_v<VectorT>
			&& !std::is_same_v<float, ValueT>)
	[[nodiscard]] double
	distance(const VectorT& b) const {
		return sqrt(static_cast<double>((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y)));
	}

	template<class VectorT>
		requires (std::is_class_v<VectorT>
			&& std::is_same_v<float, ValueT>)
	[[nodiscard]] float
	distance(const VectorT& b) const {
		return sqrtf(static_cast<float>((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y)));
	}

	// Class operators
	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2&
	operator=(
		const VectorT& b) {
		x = b.x;
		y = b.y;

		return *this;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2&
	operator+=(const VectorT& b) {
		x += b.x;
		y += b.y;

		return *this;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2&
	operator-=(const VectorT& b) {
		x -= b.x;
		y -= b.y;

		return *this;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2&
	operator*=(const VectorT& b) {
		x *= b.x;
		y *= b.y;

		return *this;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2&
	operator/=(const VectorT& b) {
		x /= b.x;
		y /= b.y;

		return *this;
	}

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2
	operator+(const VectorT& b) const { return {x + b.x, y + b.y}; }

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2
	operator-(const VectorT& b) const { return {x - b.x, y - b.y}; }

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2
	operator*(const VectorT& b) const { return {x * b.x, y * b.y}; }

	template<class VectorT>
		requires vector2_like<ValueT, VectorT>
	vector2
	operator/(const VectorT& b) const { return {x / b.x, y / b.y}; }

	// Basic operators
	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2&
	operator=(NumberT b) {
		x = b;
		y = b;

		return *this;
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	bool
	operator==(vector2<NumberT> b) const { return x == b.x && y == b.y; }

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2&
	operator+=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x += _b;
		y += _b;

		return *this;
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2&
	operator-=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x -= _b;
		y -= _b;

		return *this;
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2&
	operator*=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x *= _b;
		y *= _b;

		return *this;
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2&
	operator/=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x /= _b;
		y /= _b;

		return *this;
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2
	operator+(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x + b, y + b};
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2
	operator-(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x - b, y - b};
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2
	operator*(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x * b, y * b};
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector2
	operator/(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x / b, y / b};
	}
};

using vector2f = vector2<float>;
using vector2d = vector2<double>;
using vector2i = vector2<int>;

#endif //ZENO_URPROJECT_REWRITE_VECTOR2_H
