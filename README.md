# Personal & Academic Projects

## Overview

A curated collection of projects from personal interests and academic coursework,
primarily in C with some Python and Jupyter. Topics span numerical methods,
algorithms, low-level optimization, and probabilistic modeling.

## Table Of Contents

- [Algorithms and Data Structures](https://github.com/atahancetindemir/projects/tree/main/algorithms-data-structures)
- [Debugging](https://github.com/atahancetindemir/projects/tree/main/debugging)
- [Machine Learning](https://github.com/atahancetindemir/projects/tree/main/machine-learning)
- [Math Puzzles](https://github.com/atahancetindemir/projects/tree/main/math-puzzles)
- [Number Theory](https://github.com/atahancetindemir/projects/tree/main/number-theory)
- [Numerical Analysis](https://github.com/atahancetindemir/projects/tree/main/numerical-analysis)
- [Optimization](https://github.com/atahancetindemir/projects/tree/main/optimization)
- [Probability and Statistics](https://github.com/atahancetindemir/projects/tree/main/probability-statistics)

## Project Categories

### Algorithms and Data Structures `C`

- **Arrays** array ADT with insert, delete, search, reverse, and min/max operations
- **Matrices** scalar operations, transpose, multiply, and triangular/diagonal sums on 3×3 matrices
- **Linked Lists** singly linked list with positional insert/delete and sorted insertion
- **Prime Numbers** trial division up to √n that returns the result, smallest factor, and elapsed time
- **Fibonacci Numbers** iterative nth Fibonacci with timing

### Debugging `Python`

- **Debugger Structure** interactive debugger with step, continue, print, breakpoints, and watchpoints via sys.settrace
- **Delta Debugger** token-based ddmin that minimizes a failing input to its smallest reproducer
- **Cause Chain Effect** compares variable states between a passing and failing run, then uses ddmin to isolate which state difference causes the bug

### Machine Learning `Jupyter`

- **Heart Disease Prediction** binary classification model with feature analysis and evaluation metrics

### Math Puzzles `C`

- **Stirling's Approximation** computes Stirling's factorial approximation for a given n
- **Earthquake Energy Calculation** takes two Richter magnitudes and computes energy release, their ratio, and TNT/Hiroshima/Nagasaki equivalents
- **Car and Fly Problem** solves the classic bouncing-fly problem using both a direct formula and a numerical step simulation

### Number Theory `C`

- **Collatz Conjecture** runs the 3n+1 sequence on arbitrary-precision big numbers using custom digit-level arithmetic, covering the range 2¹ to 2²³
- **Collatz Second Approach** fast range verifier using bit manipulation (`__builtin_ctzll`) starting from a precomputed lower bound (~2³⁶)
- **Dudeney Numbers** finds all j where j equals the digit sum of j^i, scanning powers from 0 to 99

### Numerical Analysis `C`

- **Euler's Method** first-order ODE solver with fixed step size
- **Heun's Method** second-order predictor-corrector improvement over Euler
- **Methods Compared To The Analytic Solution** side-by-side absolute and relative error table for both methods against the exact solution
- **Numerical Differentiation** finite difference approximations with error analysis
- **Numerical Integration** trapezoidal and Simpson's rule implementations

### Optimization `C`

- **Cache Friendly Matrix Multiplication** demonstrates how access patterns affect cache performance
- **Loop Unrolling** manual unrolling to reduce branch overhead and improve throughput
- **Row/Column Access Patterns** benchmarks row-major vs column-major traversal costs
- **Vectorized Strassen's Matrix Multiplication** Strassen's divide-and-conquer algorithm with AVX2 SIMD at the base case

### Probability and Statistics `C` `Python`

- **Approximation to Pi** Monte Carlo estimation of π by counting random points inside the unit circle
- **Rendezvous Problem** Monte Carlo simulation of the probability two friends meet given random arrival times in a 1-hour window with a 15-minute wait
- **Consecutiveness Model** expected trials until N consecutive identical outcomes, verified against the closed-form solution
- **Financial Calculations** compares two deposit options (different period and rate) over their LCM period with 15% tax applied
- **Expected Lottery Probability** full probability matrix with CSV output and Python visualization