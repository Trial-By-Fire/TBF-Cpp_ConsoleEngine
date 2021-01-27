#pragma once



#ifndef _FLOATCMP
#define _FLOATCMP



// Includes

#include <float.h>
#include <math.h>
#include <stdbool.h>



// Public



// Macros

typedef float  float32;
typedef double float64;



// Functions

// Float 32

bool Float32_ApproxEqual(float32 _first, float32 _second);

bool Float32_ApproxGreater(float32 _first, float32 _second);

bool Float32_ApproxLess(float32 _first, float32 _second);

// Float 64

bool Float64_ApproxEqual(float64 _first, float64 _second);

bool Float64_ApproxGreater(float64 _first, float64 _second);

bool Float64_ApproxLess(float64 _first, float64 _second);

#endif _FLOATCMP