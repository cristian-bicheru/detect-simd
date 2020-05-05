===================================
Detect-SIMD |version| Documentation
===================================

Detect-SIMD is a Python library implementing a single function `detect`_ which
detects the SIMD capabilities of the user's CPU/OS. This is useful for building
vectorized packages.

.. _detect: https://detect-simd.readthedocs.io/en/latest/#detect

Detect
###################################
.. py:function:: detect_simd.detect() -> dict
   
   Detect CPU SIMD Capabilities.

   :return: Dictionary containing each SIMD instruction set. Each instruction set maps to either a 0 or a 1 indicating if it is supported by the user's computer.
   :rtype: dict


Detectable SIMD Instruction Sets
###################################
  * MMX
  * x64
  * ABM
  * RDRAND
  * BMI1
  * BMI2
  * ADX
  * PREFETCHWT1
  * SSE
  * SSE2
  * SSE3
  * SSSE3
  * SSE41
  * SSE42
  * SSE4a
  * AES
  * SHA
  * AVX
  * XOP
  * FMA3
  * FMA4
  * AVX2
  * AVX512F
  * AVX512CD
  * AVX512PF
  * AVX512ER
  * AVX512VL
  * AVX512BW
  * AVX512DQ
  * AVX512IFMA
  * AVX512VBMI


Issue Tracking and Feature Requests
###################################
  * Report issues and request features on our `GitHub`_.

.. _GitHub: https://github.com/cristian-bicheru/detect-simd/issues
