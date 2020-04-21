try:
    from detect_simd.core import detect
except ImportError:
    raise ImportError("Run setup.py to build library before importing.")

__all__ = ["detect_simd.core"]
