## Importing MP files

Configuration file format:
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