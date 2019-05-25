Mapsoft2 can read and write MP files.


Reading:
- Header (`[IMG ID]` section) is required
- Only `[POI]`, `[POLYLINE]`, `[POLYGON]`,
  `[RGN10]`, `[RGN20]`, `[RGN30]`, and `[RGN40]`
  sections are suppored, other sections are skipped.
- Comments before header are attached to MP comments,
  comments before other sections are attached to corresponding objects.
- In the header only `ID`, `Name`, `Codepage` fields are supported.
- Section names and keywords are case insensitive.
- Text fields are converted according to the `Codepage`.
- For objects only `Type`, `Data#` (or `Origin#`), `Label`,
  `Direction`, and `EndLevel` fields are supported.
- When reading a few files the header will be overwritten and
  objects will be added.

Writing:
- Only sections `[IMG ID]`, `[POI]`, `[POLYLINE]` and `[POLYGON]` are written.



