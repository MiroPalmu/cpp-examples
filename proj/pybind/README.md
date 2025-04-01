When using pybind11 as submodule it has to be installed first.

```shell
cmake -GNinja -DCMAKE_INSTALL_PREFIX=<install_path> -Bbuild -DBYBIND11_TEST=OFF .
cd build
ninja install
```

Then for meson to find it, we can do:

```shell
CMAKE_PREFIX_PATH=<install_path> meson setup build
```
