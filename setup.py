#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import platform
from setuptools import setup, Extension
import subprocess
import sys


# Set up the build environment.
try:
    llvm_home = os.environ['LLVM_HOME']
    llvm_config = os.path.join(llvm_home, 'bin', 'llvm-config')
    llvm_cflags = subprocess.check_output([llvm_config, '--cxxflags']).split()
    llvm_ldflags = subprocess.check_output([llvm_config, '--ldflags']).split()

    if sys.version_info.major >= 3:
        llvm_cflags = [p.decode('utf-8') for p in llvm_cflags]
        llvm_ldflags = [p.decode('utf-8') for p in llvm_ldflags]

    setup(
        name='sealang',
        version='3.9.dev258341',
        description='An extended set of Python bindings for libclang',
        long_description=open('README.rst').read(),
        url='http://github.com/pybee/sealang',
        license='License :: OSI Approved :: University of Illinois/NCSA Open Source License',
        classifiers=[
            'Intended Audience :: Developers',
            'License :: OSI Approved :: University of Illinois/NCSA Open Source License',
            'Programming Language :: Python',
            'Development Status :: 5 - Production/Stable',
            'Topic :: Software Development :: Compilers',
            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 2',
            'Programming Language :: Python :: 2.7',
        ],
        keywords=['llvm', 'clang', 'libclang'],
        author='LLVM team and Russell Keith-Magee',
        author_email='russell@keith-magee.com',
        packages=[
            'clang',
            'sealang'
        ],
        ext_modules=[
            Extension(
                'sealang',
                sources=['sealang/sealang.cpp'],
                libraries=[
                    'clangAST', 'clangLex', 'clangBasic', 'LLVMSupport',
                    'm', 'z', 'pthread', 'curses'
                ],
                extra_compile_args=llvm_cflags,
                extra_link_args=llvm_ldflags,
            ),
        ],
        # if use nose.collector, many plugins not is avaliable
        # see: http://nose.readthedocs.org/en/latest/setuptools_integration.html
        test_suite='nose.collector',
        tests_require=['nose']
    )

except KeyError:
    print("Unable to set up build environment. Have you installed LLVM and set LLVM_HOME?")
    print('')
    if platform.system() == 'Darwin':
        print("Using homebrew:")
        print("    brew install llvm --with-clang --with-asan")
        print("    export LLVM_HOME=/usr/local/opt/llvm")
        print("    export DYLD_LIBRARY_PATH=$LLVM_HOME/lib")
    elif platform.dist()[0] == 'Ubuntu':
        print("    curl http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -")
        print("    echo \"deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main\" | sudo tee -a /etc/apt/sources.list")
        print("    sudo apt-get update -y")
        print("    sudo apt-get install libclang-3.7-dev llvm-3.7-dev -y")
        print("    export LLVM_HOME=/usr/lib/opt/llvm")
        print("    export LD_LIBRARY_PATH=$LLVM_HOME/lib")
    print('')
