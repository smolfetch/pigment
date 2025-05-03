
<img align="right" width="26%" src="./misc/logo.png">

Pigment
===

A C++ library for working with colors

---

## Installation

### CMake

```cmake
FetchContent_Declare(
  pigment
  GIT_REPOSITORY https://github.com/bresilla/pigment.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(pigment)


target_link_libraries(<lib/bin> PRIVATE pigment::pigment)
```

---
