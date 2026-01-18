#include "Random.h"

using uint = unsigned int;

double floor(double n) {
	double res = (double)((int)n);
	if (n < 0) {
		res = res - 1.0;
	}
	return res;
}

double fract(double n) {
	double res = n - floor(n);
	if (n < 0) {
		res = 1 - res;
	}
	return res;
}

// extern "C" MYLIBRARY_API
template<typename T>
struct vec2 {
	T x, y;
	vec2(): x(0), y(0) {};
	vec2(T x): x(x), y(x) {};
	vec2(T x, T y): x(x), y(y) {};

	vec2 operator+(const vec2& other) const {
		return vec2(x + other.x, y + other.y);
	}
	vec2 operator-(const vec2& other) const {
		return vec2(x - other.x, y - other.y);
	}
	vec2 operator*(const vec2& other) const {
		return vec2(x * other.x, y * other.y);
	}
	vec2 operator/(const vec2& other) const {
		return vec2(x / other.x, y / other.y);
	}
	vec2 operator&(const vec2& other) const {
		return vec2(x & other.x, y & other.y);
	}
	vec2 operator|(const vec2& other) const {
		return vec2(x | other.x, y | other.y);
	}
	vec2 operator^(const vec2& other) const {
		return vec2(x ^ other.x, y ^ other.y);
	}
	vec2 operator<<(const vec2& other) const {
		return vec2(x << other.x, y << other.y);
	}
	vec2 operator>>(const vec2& other) const {
		return vec2(x >> other.x, y >> other.y);
	}

	vec2 operator+(const T n) const {
		return vec2(x + n, y + n);
	}
	vec2 operator-(const T n) const {
		return vec2(x - n, y - n);
	}
	vec2 operator*(const T n) const {
		return vec2(x * n, y * n);
	}
	vec2 operator/(const T n) const {
		return vec2(x / n, y / n);
	}
	vec2 operator&(const T n) const {
		return vec2(x & n, y & n);
	}
	vec2 operator|(const T n) const {
		return vec2(x | n, y | n);
	}
	vec2 operator^(const T n) const {
		return vec2(x ^ n, y ^ n);
	}
	vec2 operator<<(const T n) const {
		return vec2(x << n, y << n);
	}
	vec2 operator>>(const T n) const {
		return vec2(x >> n, y >> n);
	}
};

uint part1by1(uint x) {
	x = (x & 0x0000ffffu);
    x = ((x ^ (x << 8u)) & 0x00ff00ffu);
    x = ((x ^ (x << 4u)) & 0x0f0f0f0fu);
    x = ((x ^ (x << 2u)) & 0x33333333u);
    x = ((x ^ (x << 1u)) & 0x55555555u);
    return x;
}

uint compact1by1(uint x) {
	x = (x & 0x55555555u);
    x = ((x ^ (x >> 1u)) & 0x33333333u);
    x = ((x ^ (x >> 2u)) & 0x0f0f0f0fu);
    x = ((x ^ (x >> 4u)) & 0x00ff00ffu);
    x = ((x ^ (x >> 8u)) & 0x0000ffffu);
    return x;
}

uint pack_morton2x16(vec2<uint> v) {
	return part1by1(v.x) | (part1by1(v.y) << 1);
}

vec2<uint> unpack_morton2x16(uint p) {
	return vec2<uint>(compact1by1(p), compact1by1(p >> 1));
}

uint inverse_gray32(uint n) {
	n = n ^ (n >> 1);
    n = n ^ (n >> 2);
    n = n ^ (n >> 4);
    n = n ^ (n >> 8);
    n = n ^ (n >> 16);
    return n;
}

// https://www.shadertoy.com/view/llGcDm
int hilbert(vec2<int> p, int level) {
	int d = 0;
	for (int k = 0; k < level; k++) {
		int n = level - k - 1;
		vec2<int> r = (p >> n) & 1;
		d += ((3*r.x) ^ r.y) << (2 * n);
		if (r.y == 0) { if (r.x == 1) { p = p*(-1)+((1<<n)-1);} p = vec2(p.y, p.x); }
	}
	return d;
}

vec2<int> ihilbert(int i, int level) {
	vec2<int> p = vec2<int>(0);
	for (int k = 0; k < level; k++) {
		vec2<int> r = vec2<int>(i >> 1, i ^ (i >> 1)) & 1;
		if (r.y == 0) { if (r.x == 1) { p = p*(-1)+((1 << k)-1); } p = vec2(p.y, p.x); }
		p = p + (r << k);
		i = i >> 2;
	}
	return p;
}

// knuth's multiplicative hash function (fixed point R1)
uint kmhf(uint x) {
    return 0x80000000u + 2654435789u * x;
}

uint kmhf_inv(uint x) {
    return (x - 0x80000000u) * 827988741u;
}

uint hilbert_r1_blue_noise(vec2<uint> p) {
	#if 1
		uint x = (uint)(hilbert(vec2<int>(p.x, p.y), 17) % (1u << 17u));
	#else
		// p = p ^ (p >> 1);
		uint x = pack_morton2x16(p) % (1u << 17);
		// x = x ^ (x >> 1)
		x = inverse_gray32(x);
	#endif
	#if 0
		// based on http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
	    const double phi = 2.0/(sqrt(5.0)+1.0);
		return fract(0.5+phi*(double)(x));
	#else
		x = kmhf(x);
		return x;
	#endif
}

double hilbert_r1_blue_noisef(vec2<uint> p) {
	uint x = hilbert_r1_blue_noise(p);
	#if 0
		return (double)(x >> 24) / 256.0;
	#else
		return (double)(x) / 4294967296.0;
	#endif
}

vec2<uint> hilbert_r1_blue_noise_inv(uint x) {
	x = kmhf_inv(x);
	vec2<int> temp = ihilbert((int)x, 17);
	return vec2<uint>(temp.x, temp.y);
}

double hash12(vec2<int> p) {
	double p1 = fract(p.x * 0.1031);
	double p2 = fract(p.y * 0.1031);
	double p3 = fract(p.x * 0.1031);

	double p4 = p1*(p2+33.33) + p2*(p3+33.33) + p3*(p1+33.33);
	p1 += p4;
	p2 += p4;
	p3 += p4;

	return fract((p1 + p2) * p3);
}


double fractHash(int x, int y) {
	return hash12(vec2<int>(x,y));
}

double blueHash(int x, int y) {
	return hilbert_r1_blue_noisef(vec2<uint>(x,y));
}
