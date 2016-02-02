#!/usr/bin/env python
# -*- coding: utf-8 -*-
from setuptools import setup, Extension

setup(
    name="sealang",
    version="3.9.dev258341",
    description="An extended set of Python bindings for libclang",
    long_description=open("README.rst").read(),
    url="http://github.com/pybee/sealang",
    license="License :: OSI Approved :: University of Illinois/NCSA Open Source License",
    classifiers=[
        "Intended Audience :: Developers",
        "License :: OSI Approved ::  University of Illinois/NCSA Open Source License",
        "Programming Language :: Python",
        "Development Status :: 5 - Production/Stable",
        "Topic :: Software Development :: Compilers"
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
    ],
    keywords=["llvm", "clang", "libclang"],
    author="LLVM team and Russell Keith-Magee",
    packages=["clang"],
    ext_modules=[
        Extension(
            "sealang",
            sources=["sealang/sealang.cpp"],
            libraries=[
                'clangAST', 'clangBasic', 'clangLex', 'LLVMSupport',
                'm', 'z', 'pthread', 'curses'
            ],
        ),
    ],
    # if use nose.collector, many plugins not is avaliable
    # see: http://nose.readthedocs.org/en/latest/setuptools_integration.html
    test_suite="nose.collector",
    tests_require=['nose']
)
