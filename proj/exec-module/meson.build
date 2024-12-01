# Copyright (C) 2024 Miro Palmu.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.

project(
    'gnulander',
    'cpp',
    version: '0.0.1',
    default_options : ['warning_level=3', 'werror=true', 'optimization=3', 'cpp_std=c++26']
)

if not meson.is_subproject()
    add_global_arguments('-fconcepts-diagnostics-depth=10', language : 'cpp')
endif

cxx = meson.get_compiler('cpp')
if cxx.get_id() != 'gcc'
  error('Requires gcc!')
endif

std_module = custom_target('std-module',
                           # Should be this, but meson wont allow subdirs in output.
                           # output: ['gcm.cache/std.gcm', 'std.o'],
                           # Should be this, same output breaks ninja.
                           # output: ['gcm.cache', 'std.o'],
                           output: ['std.o'],
                           command: [cxx.cmd_array(),
                                     '-std=c++26',
                                     '-fmodules',
                                     '-fsearch-include-path',
                                     '-c', 'bits/std.cc',
                                    ],
                           )

exec_module = custom_target('exec-module',
                            input: files('exec-module.cpp'),
                            depends: [std_module],
                            # Should be, but meson wont allow subdirs in output.
                            # output: ['gcm.cache/std.gcm', 'std.o'],
                            output: ['gcm.cache', 'exec-module.o'],
                            command: [cxx.cmd_array(),
                                      '-Wall', '-Werror', '-Wextra', '-Wconversion',
                                      '-std=c++26', '-fmodules',
                                      '-c', '@INPUT@',
                                     ],
                            )

executable('foo',
           files('main.cpp'),
           std_module,
           exec_module,
           cpp_args: ['-fmodules'],
          )
