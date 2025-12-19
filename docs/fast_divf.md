# Fast Single-Precision Float Division on Xtensa

## Overview
This document describes a performance optimization used in the decode path on Xtensa targets when `CONFIG_FAST_DIVF` is enabled. The standard IEEE754 single-precision
division routine `__divsf3` is replaced with a faster, reciprocal-based implementation.

This optimization trades IEEE754 compliance for reduced instruction count and improved latency.

## Original Implementation
The default Xtensa `__divsf3` implementation performs a fully IEEE754 compliant division, handling normalization, special values, and rounding.

```asm
    entry	    a1, 16
    wfr	        f1, a2
    wfr	        f2, a3
    div0.s	    f3, f2
    nexp01.s	f4, f2
    const.s	    f5, 1
    maddn.s	    f5, f4, f3
    mov.s	    f6, f3
    mov.s	    f7, f2
    nexp01.s	f2, f1
    maddn.s	    f6, f5, f6
    const.s	    f5, 1
    const.s	    f0, 0
    neg.s	    f8, f2
    maddn.s	    f5, f4, f6
    maddn.s	    f0, f8, f3
    mkdadj.s	f7, f1
    maddn.s	    f6, f5, f6
    maddn.s	    f8, f4, f0
    const.s	    f3, 1
    maddn.s	    f3, f4, f6
    maddn.s	    f0, f8, f6
    neg.s	    f2, f2
    maddn.s	    f6, f3, f6
    maddn.s	    f2, f4, f0
    addexpm.s	f0, f7
    addexp.s	f6, f7
    divn.s	    f0, f2, f6
    rfr	        a2, f0
    retw.n
```

## Optimized Implementation
Uses reciprocal approximation with two Newton–Raphson refinements, not IEEE754 compliant.
The decode path does not require **6–7** digits of floating-point precision; two Newton-Raphson iterations provide sufficient accuracy while significantly reducing instruction count.

#### Accurancy
- ~1–2 ulp relative error in normal ranges
- Not IEEE754 compliant
- Undefined behavior for:
  - NaN
  - ±Inf
  - Division by zero
```asm
    entry	    a1, 48
    wfr	        f0, a2
    s32i.n	    a3, a1, 0
    wfr	        f3, a3
    recip0.s	f1, f3
    const.s	    f2, 1
    msub.s	    f2, f3, f1
    madd.s	    f1, f1, f2
    const.s	    f2, 1
    msub.s	    f2, f3, f1
    maddn.s	    f1, f1, f2
    mul.s	    f0, f1, f0
    rfr	        a2, f0
    retw.n
```
