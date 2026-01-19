#include <iostream>
#include <string>
#include <cstring>

using uint = unsigned int;

double floor(double n);
double fract(double n);

template<typename T>
struct vec2;

uint part1by1(uint x);
uint compact1by1(uint x);
uint pack_morton2x16(vec2<uint> v);
vec2<uint> unpack_morton2x16(uint p);
uint inverse_gray32(uint n);

int hilbert(vec2<int> p, int level);
vec2<int> ihilbert(int i, int level);

uint kmhf(uint x);
uint kmhf_inv(uint x);

uint hilbert_r1_blue_noise(vec2<uint> p);
double hilbert_r1_blue_noisef(vec2<uint> p);
vec2<uint> hilbert_r1_blue_noise_inv(uint x); 

double hash12(vec2<int> p);

double fractHash(int x, int y);
double blueHash(int x, int y);
