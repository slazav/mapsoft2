## Err class

Human-readable text exceptions (with optional error codes)

- Throwing an error:
```c
throw Err() << "some message";
```

- Catching and printing error:
```c
catch (Err E){ cerr << "Error: " << E.str() << "\n"; }
```

- Integer error code can be set as an optional constructor parameter
  and extracted by `code` method: `E.code()`. Default code is -1.
```c
try {throw Err(1); } catch (Err E) {int code=E.code();}
```

## assert_err macro

```
#include "err/assert_err.h"
assert_err(<code>, <expected error>)
```


------------------
## Changelog:

2019.10.10 V.Zavjalov 1.2:
- Add assert_eq, assert_deq, and assert_feq macro
- Print more information in assert_*

2019.08.16 V.Zavjalov 1.1:
- Add assert_err macro

2019.05.02 V.Zavjalov 1.0:
- First version (used widely in many of my projects)
