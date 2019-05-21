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

------------------
## Changelog:

2019.05.02 V.Zavjalov 1.0:
- First version (used widely in many of my projects)
