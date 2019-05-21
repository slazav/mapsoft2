-----------------
## Opt class

Opt class is a `map<string,string>` container with functions for
getting/putting values of arbitrary types. Data types should have `<<`,
`>>` operators and a constructor without arguments.

- Converting any object to `std::string` and back:
```c
std::string str = type_to_str(v);
type v = str_to_type<type>(str);
```
Here `v` is some object of type `type`. All types except `std::string` are
passed through `std::stringstream` and `<<` or `>>` operators. For type `int`
it is possible to read hex values (prefixed by `0x`). For writing hex values into
string one can use function
```c
str = type_to_str_hex(int_v);
```

- Creating Opt class, putting and extracting values:
```c
Opt o;
o.put<type>("key", v);
o.put_hex("key", int_v);
v = o.get<type>("key", default);
```
Here `"key"` is a string key which is used to access data.
If no value is set for this key, the `def` object is returned.

- Merging options (adding options from opts1, replacing old values):
```c
opts.put(opts1)
```

Opt object can be converted to a string and back (and thus used inside Opt class).
String representation is a JSON object with string fields.


-----------------
## Changelog:

2019.05.20 V.Zavjalov 1.3:
- put(Opt) method for merging options

2018.09.17 V.Zavjalov 1.2:
- support for hex integers (with 0x prefix)

2018.08.07 V.Zavjalov 1.1
- fix error with string values
- fix interface

2018.04.08 V.Zavjalov 1.0:
- extract from original mapsoft package
