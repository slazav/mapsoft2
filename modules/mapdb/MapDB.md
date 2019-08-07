## VMapObj -- a single map object

A dMultiLine structure with additional fields:

`VMapObjClass cl` -- object class: POINT, LINE, POLYGON, default: POINT
`int type` -- object type (like MP type), default: 0
`VMapObjDir dir` -- object direction: NO, FRW, BCK, default: NO
`float angle`  -- object angle, degrees, default 0
`std::string name` -- object name (to be printed on map labels)
`std::string comm` -- object comment
`std::string src` -- object source

Methods for packing and anpacking the object to a string (compact representation
for keepint in the DB storage):

```c++
std::string pack() const;
void unpack(const std::string & s);
```

Comparison operators: ==, !=, <, >, <=, >=

--------

## VMap -- a storage for VMapObj


### Import from MP:
```c++
void VMap::import_mp( const string & mp_file, const Opt & opts);
```
#### Options:

- `config` - Configuration file. Format is described below.

- `cnv_point`, `cnv_line`, `cnv_polygon` -- conversions for point, line,
  polygon objects. Overrides settings from the configuration file.
  Value should be `iLine` object where each point sets a pair of
  integer numbers. Example: `--cnv_point [[0x4355,0x2222],[0,0]]`

- `data_level` -- data level (overrides configuration file).

#### Configuration file format:

- Comments `#`, escape character `\`, quotes `"` or `'`.
- Case not important.
- Hex numbers (with 0x prefix) are allowed.

- `(POI|POINT) <N> <M>` -- convert point object of type `N`
  to type `M`. If `N` is 0 then any type is converted.
  If `M` is 0 then output type will be same as input type.

- `(LINE|POLYLINE) <N> <M>` -- convert line object.

- `POLYGON <N> <M>` -- convert a polygon.

- `LEVEL <N>` -- use level <N> of data. Default is 0.

By default (if configuration file if empty) the configuration is
``
POINT 0 0
LINE 0 0
POLYGON 0 0
``
This means that all types are converted without change.


### Export to MP:
```c++
void VMap::export_mp(const string & mp_file, const Opt & opts);

```

#### Options:

- `config` - configuration file. Same format as for import_mp, but without
             LEVEL setting.

- `cnv_point`, `cnv_line`, `cnv_polygon` -- conversions for point, line,
  polygon objects. Overrides settings from the configuration file.
  Value should be `iLine` object where each point sets a pair of
  integer numbers. Example: `--cnv_point [[0x4355,0x2222],[0,0]]`

#### TODO: many MP-specific parameters: ID, EndLevel etc.

--------

