# Using hpx

Initially this project tried to use meson as build system,
as one can depend on libraries using pkg-config (.pc) or cmake (add_library).
However both of these methods were not feasible, so cmake it is.

## Pkg-config

It seems that hpx .pc files are incomplete and require manually adding
additional linking flags for full functionality and this is hard (not supported) in meson.

## Cmake dependency

Hpx recommends using add_hpx_executable cmake macro to create cmake executable target.
This can not be done in meson.
