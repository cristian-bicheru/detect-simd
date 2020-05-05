from Cython.Distutils import build_ext
try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

core = Extension('detect_simd/core',
                   sources=['detect_simd/src/detect_simd.c'])

setup(name = 'detect_simd',
      packages = ["detect_simd"],
      version = '0.2',
      license = 'MIT',
      license_file = "LICENSE.md",
      description = "Python library to detect CPU SIMD capabilities.",
      author = "Cristian Bicheru",
      author_email = "c.bicheru0@gmail.com",
      maintainer = "Cristian Bicheru",
      maintainer_email ="c.bicheru0@gmail.com",
      download_url = 'https://github.com/cristian-bicheru/detect-simd/archive/v0.2.tar.gz',
      keywords = ['cpu', 'detect', 'simd', 'python3', 'sse', 'avx', 'avx2', 'avx512', 'fma'],
      classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: System :: Hardware',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
      ],
      project_urls = {
        'Documentation': 'https://detect-simd.readthedocs.io/',
        'Bug Reports': 'https://github.com/cristian-bicheru/detect-simd/issues',
        'Source': 'https://github.com/cristian-bicheru/detect-simd',
      },
      cmdclass = {'build_ext': build_ext},
      ext_modules=[core])
