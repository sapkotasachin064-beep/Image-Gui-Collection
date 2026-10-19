//
// Created by n08i40k on 14.12.2023.
//

#ifndef ZENO_URPROJECT_REWRITE_VECTOR3_H
#define ZENO_URPROJECT_REWRITE_VECTOR3_H

#include <cstdint>
#include <stdexcept>

template<typename ValueT>
class vector3 {
public:
	ValueT x, y, z;

	vector3() : vector3(0, 0, 0) {
	}

	template<typename NumberT>
	vector3(NumberT x,
	        NumberT y,
	        NumberT z) {
		this->x = static_cast<ValueT>(x);
		this->y = static_cast<ValueT>(y);
		this->z = static_cast<ValueT>(z);
	}

	template<typename NumberT>
	explicit
	vector3(vector3<NumberT>&& other_type) noexcept : vector3(
		                                                other_type.x,
		                                                other_type.y,
		                                                other_type.z) {
	}

	template<class VectorT>
	explicit
	vector3(const VectorT& custom_vector) : vector3(
		                                      custom_vector.x,
		                                      custom_vector.y,
		                                      custom_vector.z) {
	}

	[[nodiscard]] constexpr ValueT&
	operator[](const uint8_t index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::range_error("Index out of bounds!");
		}
	}
	[[nodiscard]] constexpr ValueT
	operator[](const uint8_t index) const {
		return operator[](index);
	}

	[[nodiscard]] bool
	is_zero() const {
		return x == 0 && y == 0 && z == 0;
	}

	[[nodiscard]] double
	magnitude() const {
		return sqrt(static_cast<double>(x * x + y * y + z * z));
	}

	void
	normalize() {
		if (const long double sum = x * x + y * y + z * z;
			sum > 0) {
			long double d = 1.0 / sqrt(sum);
			x = d * x;
			y = d * y;
			z = d * z;
		} else
			x = 1.0;
	}

	template<class VectorT>
		requires std::is_class_v<VectorT>
	[[nodiscard]] vector3
	cross(const VectorT& b) const {
		return {
			y * b.z - z * b.y,
			z * b.x - x * b.z,
			x * b.y - y * b.x
		};
	}

	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	[[nodiscard]] vector3<NumberT>
	convert() const {
		return {x, y, z};
	}

	template<typename VectorT>
		requires std::is_class_v<VectorT>
	[[nodiscard]] VectorT
	convert() const {
		return VectorT{x, y, z};
	}

	template<class VectorT>
		requires std::is_class_v<VectorT>
	[[nodiscard]] double
	distance(const VectorT& b) const {
		return sqrt(static_cast<double>((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y) + (z - b.z) * (z - b.z)));
	}

	// Basic operators
	template<typename NumberT>
		requires std::is_arithmetic_v<NumberT>
	vector3&
	operator=(NumberT b) {
		x = b;
		y = b;
		z = b;

		return *this;
	}

	template<typename NumberT>
	vector3&
	operator+=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x += _b;
		y += _b;
		z += _b;

		return *this;
	}
	template<typename NumberT>
	vector3&
	operator-=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x -= _b;
		y -= _b;
		z -= _b;

		return *this;
	}
	template<typename NumberT>
	vector3&
	operator*=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x *= _b;
		y *= _b;
		z *= _b;

		return *this;
	}
	template<typename NumberT>
	vector3&
	operator/=(NumberT b) {
		auto _b = static_cast<ValueT>(b);
		x /= _b;
		y /= _b;
		z /= _b;

		return *this;
	}

	template<typename NumberT>
	vector3
	operator+(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x + b, y + b, z + b};
	}
	template<typename NumberT>
	vector3
	operator-(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x - b, y - b, z - b};
	}
	template<typename NumberT>
	vector3
	operator*(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x * b, y * b, z * b};
	}
	template<typename NumberT>
	vector3
	operator/(NumberT b) const {
		auto _b = static_cast<ValueT>(b);

		return {x / b, y / b, z / b};
	}

	// Class operators
	template<class VectorT>
		requires std::is_class_v<VectorT>
	vector3&
	operator=(
		const VectorT& b) {
		x = b.x;
		y = b.y;
		z = b.z;

		return *this;
	}

	vector3&
	operator=(const vector3& b) {
		x = b.x;
		y = b.y;
		z = b.z;

		return *this;
	}

	vector3&
	operator+=(const vector3& b) {
		x += b.x;
		y += b.y;
		z += b.z;

		return *this;
	}
	vector3&
	operator-=(const vector3& b) {
		x -= b.x;
		y -= b.y;
		z -= b.z;

		return *this;
	}
	vector3&
	operator*=(const vector3& b) {
		x *= b.x;
		y *= b.y;
		z *= b.z;

		return *this;
	}
	vector3&
	operator/=(const vector3& b) {
		x /= b.x;
		y /= b.y;
		z /= b.z;

		return *this;
	}

	vector3
	operator+(const vector3& b) const {
		return {x + b.x, y + b.y, z + b.z};
	}
	vector3
	operator-(const vector3& b) const {
		return {x - b.x, y - b.y, z - b.z};
	}
	vector3
	operator*(const vector3& b) const {
		return {x * b.x, y * b.y, z * b.z};
	}
	vector3
	operator/(const vector3& b) const {
		return {x / b.x, y / b.y, z / b.z};
	}
};

using vector3f = vector3<float>;
using vector3d = vector3<double>;
using vector3i = vector3<int>;

#endif //ZENO_URPROJECT_REWRITE_VECTOR3_H
