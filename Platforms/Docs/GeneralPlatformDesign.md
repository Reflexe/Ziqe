# General Platform Laters
## Layers (from the lowest level)
* The CppCore is a very minimal API that is required in order to avoid circular
  dependency between the OS layer and the Utils Layer. It provides a minimal set
  of functions required by C++ in order to work correctly:
    * Memory Managment: ZqAllocate and ZqDeallocate.
    * Types: stdint.h uintX_t types.

* The PerDriver layer contains symbols that unique for each driver. This layer
  get compiled again for each driver.
  Exanple: Entry point should be unique for each

* The Utils layer contains implementations of basic data structures and an
  alternative helper library to the STL. It doesn't interact with the OS
  and only calls the CppCore functions.

* The OS layer uses the Utils helpers and creates a implementation of the
  Ziqe OS standard for each platform. It does interact (directly and indirecly)
  with the OS.

```
 ==============================================
:               Linux | WDK | MAC              :
 =====|================|=================|=====
      |  ==============|==============   |
      | :           CppCore           :  |
      |  |=============|==============   |
 =====|==|=====   =====|==============   |
:   PerDriver  : :       Utils        :  |
 =======|======   =|=======|==========   |
        |          |  =====|=============|=====
        |          | :          OS             :
        |          |  ============|============
 =======|==========|==============|============
:                   User's Code                :
 ==============================================
```

Notes:
    1: Should PerDriver interact with the user?

