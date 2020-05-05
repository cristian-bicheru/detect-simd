#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <intrin.h>
    #define cpuid(info, x)    __cpuidex(info, x, 0)
#else
    #include <cpuid.h>
    void cpuid(int info[4], int InfoType) {
        __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
    }

    unsigned long long _xgetbv(unsigned int index) {
        unsigned int eax, edx;
        __asm__ __volatile__(
        "xgetbv;"
        : "=a" (eax), "=d"(edx)
        : "c" (index)
        );
        return ((unsigned long long) edx << 32) | eax;
    }
#endif

const char* names[31] = {
        "MMX",
        "x64",
        "ABM",
        "RDRAND",
        "BMI1",
        "BMI2",
        "ADX",
        "PREFETCHWT1",
        "SSE",
        "SSE2",
        "SSE3",
        "SSSE3",
        "SSE41",
        "SSE42",
        "SSE4a",
        "AES",
        "SHA",
        "AVX",
        "XOP",
        "FMA3",
        "FMA4",
        "AVX2",
        "AVX512F",
        "AVX512CD",
        "AVX512PF",
        "AVX512ER",
        "AVX512VL",
        "AVX512BW",
        "AVX512DQ",
        "AVX512IFMA",
        "AVX512VBMI",
};

const char pydictstr[125] = "{s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,"
                             "s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,"
                             "s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i,s:i}";

static PyObject* detect(PyObject* self, PyObject* args) {
    bool flags[31] = {0};
    

    int info[4];
    cpuid(info, 0);
    int nIds = info[0];

    cpuid(info, 0x80000000);
    unsigned nExIds = info[0];

//  Detect Features
    if (nIds >= 0x00000001) {
        cpuid(info, 0x00000001);
        flags[0] = (info[3] & ((int) 1 << 23)) != 0;
        flags[8] = (info[3] & ((int) 1 << 25)) != 0;
        flags[9] = (info[3] & ((int) 1 << 26)) != 0;
        flags[10] = (info[2] & ((int) 1 << 0)) != 0;

        flags[11] = (info[2] & ((int) 1 << 9)) != 0;
        flags[12] = (info[2] & ((int) 1 << 19)) != 0;
        flags[13] = (info[2] & ((int) 1 << 20)) != 0;
        flags[15] = (info[2] & ((int) 1 << 25)) != 0;

        flags[17] = (info[2] & ((int) 1 << 28)) != 0;
        flags[19] = (info[2] & ((int) 1 << 12)) != 0;

        flags[3] = (info[2] & ((int) 1 << 30)) != 0;
    }
    if (nIds >= 0x00000007) {
        cpuid(info, 0x00000007);
        flags[21] = (info[1] & ((int) 1 << 5)) != 0;

        flags[4] = (info[1] & ((int) 1 << 3)) != 0;
        flags[5] = (info[1] & ((int) 1 << 8)) != 0;
        flags[6] = (info[1] & ((int) 1 << 19)) != 0;
        flags[16] = (info[1] & ((int) 1 << 29)) != 0;
        flags[7] = (info[2] & ((int) 1 << 0)) != 0;

        flags[22] = (info[1] & ((int) 1 << 16)) != 0;
        flags[23] = (info[1] & ((int) 1 << 28)) != 0;
        flags[24] = (info[1] & ((int) 1 << 26)) != 0;
        flags[25] = (info[1] & ((int) 1 << 27)) != 0;
        flags[26] = (info[1] & ((int) 1 << 31)) != 0;
        flags[27] = (info[1] & ((int) 1 << 30)) != 0;
        flags[28] = (info[1] & ((int) 1 << 17)) != 0;
        flags[29] = (info[1] & ((int) 1 << 21)) != 0;
        flags[30] = (info[2] & ((int) 1 << 1)) != 0;
    }
    if (nExIds >= 0x80000001) {
        cpuid(info, 0x80000001);
        flags[1] = (info[3] & ((int) 1 << 29)) != 0;
        flags[2] = (info[2] & ((int) 1 << 5)) != 0;
        flags[14] = (info[2] & ((int) 1 << 6)) != 0;
        flags[20] = (info[2] & ((int) 1 << 16)) != 0;
        flags[18] = (info[2] & ((int) 1 << 11)) != 0;
    }

    bool osUsesXSAVE_XRSTORE = info[2] & (1 << 27) || false;
    if (osUsesXSAVE_XRSTORE && flags[17])
    {
        unsigned long long xcrFeatureMask = _xgetbv(0);
        flags[17] = (xcrFeatureMask & 0x6) == 0x6;
        if (flags[21]) {
            flags[21] = flags[17];
        }
    }
    
    osUsesXSAVE_XRSTORE = info[2] & (1 << 27) || false;
    if (osUsesXSAVE_XRSTORE && flags[22])
    {
        unsigned long long xcrFeatureMask = _xgetbv(0);
        flags[22] = (xcrFeatureMask & 0xe6) == 0xe6;

        for (int i = 23; i < 31; i++) {
            if (flags[i]) {
                flags[i] = flags[22];
            }
        }
    }


    PyObject* support = Py_BuildValue(pydictstr, names[0], flags[0],
                                      names[1], flags[1],
                                      names[2], flags[2],
                                      names[3], flags[3],
                                      names[4], flags[4],
                                      names[5], flags[5],
                                      names[6], flags[6],
                                      names[7], flags[7],
                                      names[8], flags[8],
                                      names[9], flags[9],
                                      names[10], flags[10],
                                      names[11], flags[11],
                                      names[12], flags[12],
                                      names[13], flags[13],
                                      names[14], flags[14],
                                      names[15], flags[15],
                                      names[16], flags[16],
                                      names[17], flags[17],
                                      names[18], flags[18],
                                      names[19], flags[19],
                                      names[20], flags[20],
                                      names[21], flags[21],
                                      names[22], flags[22],
                                      names[23], flags[23],
                                      names[24], flags[24],
                                      names[25], flags[25],
                                      names[26], flags[26],
                                      names[27], flags[27],
                                      names[28], flags[28],
                                      names[29], flags[29],
                                      names[30], flags[30]);
    return support;
}

static PyMethodDef SIMDMethods[] = {
        {"detect", (PyCFunction) detect, METH_VARARGS, "Detect SIMD Capabilities"},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cModPyDem = {
        PyModuleDef_HEAD_INIT,
        "core", "doc todo",
        -1,
        SIMDMethods
};

PyMODINIT_FUNC
PyInit_core(void) {
    return PyModule_Create(&cModPyDem);
};
