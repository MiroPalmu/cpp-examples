project('bybind-test', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'werror=true', 'cpp_std=c++26'])

py = import('python').find_installation(pure: false)

py.extension_module('pybind_test',
                    'example.cpp',
                    dependencies: dependency('pybind11'))
