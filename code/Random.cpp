// some of this was copied from other people and other languages
// https://www.shadertoy.com/view/tcGBRW
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


/* (MT19937)
	Mersenne Twister: a 623-dimensionally
	equidistributed uniform pseudorandom number generator
	by Makoto Matsumoto and Takuji Nishimura
	https://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/ARTICLES/mt.pdf
*/

// period params
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df // const vec a
#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

// tempering params
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N]; // array for state vector
static int mti = N + 1; // mti == N+1 means mt[N] is not initialized

// initialize the array with NONZERO seed
void sgenrand(unsigned long seed) {
	mt[0] = seed & 0xffffffff;
	for (mti = 1; mti < N; mti++) {
		mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;
	}
}

double genrand() { // unsigned long for integer generation
	unsigned long y;
	static unsigned long mag01[2] = {0x0, MATRIX_A};
	// mag01[x] = x * MATRIX_A for x=0,1

	if (mti >= N) { // generate N words at one time
		int kk;
		
		if (mti == N+1) { // if sgenrand() has not been called
			sgenrand(4357);
		}

		for (kk = 0; kk < N - M; kk++) {
			// y = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1]; // uses y before it's set
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK); // does not
			mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 1]; // does not
		}
		for (; kk < N - 1; kk++) {
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);

	return ( (double)y / (unsigned long)0xffffffff ); // reals
	// return y; // for integer generation
}


double zenry(int x, int y) { 
  static uint32_t n = 0xefc8249d;
  double h;
  
  n += x;
  h = 0.02519603282416938 * n;
  n = h;
  h -= n;
  h *= n;
  n = h;
  h -= n;
  n += h * 0x100000000ULL;
  
  n += (int)(fractHash((int)(h*256.8)+17, y) * 739 + genrand() * 0x00103470);
  h = blueHash((int)n, (int)(h*256.8)) * n;
  n = h;
  h -= n;
  h *= n;
  n = h;
  h -= n;
  n += h * 0x100000000ULL;
  
  return n * 2.3283064365386963e-10;
}

double biasedZenry(int x, int y, double b, double I) {
	double r1 = zenry(x-1, y+1);
	double r2 = zenry(x+1, y-1);

	if (b < 0.0) b = 0.0;
	if (b > 1.0) b = 1.0;
	if (I < 0.0) I = 0.0;
	if (I > 1.0) I = 1.0;

	return r1 * (1.0 - r2 * I) + b * (r2 * I);
}