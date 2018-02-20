# General Platform Laters
## Layers (from the lowest level)
* The CppCore is a very minimal API that is required in order to avoid circular
  dependency between the OS layer and the Base Layer. It provides a minimal set
  of functions required by C++ in order to work correctly:
    * Memory Managment: ZqAllocate and ZqDeallocate.
    * Types: stdint.h uintX_t types.

* The Base layer contains implementations of basic data structures and an
  alternative helper library to the STL. It doesn't interact with the OS
  and only calls the CppCore functions.

* The PerDriver layer contains symbols that unique for each driver. This layer
  get compiled again for each driver.
  Exanple: Entry point should be unique for each

* The OS layer uses the Base helpers and creates a implementation of the
  Ziqe OS standard for each platform. It does interact (directly and indirecly)
  with the OS.

```
 ==============================================
:               Linux | WDK | MAC              :
 =====|================|=================|=====
      |  ==============|===============  |
      | :           CppCore            : |
      |  =|============|=============|=  |
      |   |  ==========|===========  |   |
      |   | :         Base         : |   |
      |   |  =|========|=========|=  |   |
      |   |   |        |     ====|===|===|=====
      |   |   |        |    :        OS        :
      |   |   |   ---- | --- /========|========
 =====|===|===|==/=    |              |
:     PerDriver    :   |              |
 ========|=============|==============|========
:                   User's Code                :
 ==============================================
```

Problem:
    I want PerDriver to know about OS
    It should be possible.
    DriverContext should be a regular pointer that
    get allocated before ZqOnLoad and deallocated after
    ZQ
