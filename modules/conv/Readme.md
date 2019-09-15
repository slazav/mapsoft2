-----------------
## ConvBase class

Trivial point transformation with two factors for scaling before
and after the transformation.. Children can redefine frw_pt() and bck_pt()
methods to build more complicated transformations. Scaling factors
are applied in following way: `dst = f(src*k_src)*k_dst`,
`src = f^(-1)(dst/k_dst)/k_src`

Note that in some cases forward and backward conversions are different
(accuracy is always calculated in source units).

- `ConvBase()` -- Constructor.

- `frw_pt(dPoint &), bck_pt(dPoint &)` -- Functions to be redefined in children,
   forward and backwart in-place point conversion. By default is is
   just a rescaling with `rescale_src*rescale_dst` factor.

- `frw(dPoint &), bck(dPoint &), frw(dLine &), bck(dLine &),
   frw(dMultiLine &), bck(MultidLine &)` -- Convert points
    (save as frw_pt, bck_pt), lines and multilines (without changing number of points).

- `dLine frw_acc(const dLine & l, double acc) const`
- `dLine bck_acc(const dLine & l, double acc) const` --
  Convert a line. Each segment can be divided to provide
  accuracy `<acc>` in source units (both for `frw_acc` and `bck_acc`).

- `dMultiLine frw_acc(const dMultiLine & l, double acc) const`
- `dMultiLine bck_acc(const dMultiLine & l, double acc) const` --
  Convert a MultiLine. Each sub-line is converted by frw_acc/bck_acc.
  Accuracy `<acc>` is in source units (both for `frw_acc` and `bck_acc`).

- `dRect frw_acc(const dRect & R, double acc) const`,
- `dRect bck_acc(const dRect & R, double acc) const` --
  Convert a rectagle and return bounding box of resulting figure.
  Accuracy `<acc>` is measured in source units (both for frw_acc and bck_acc).

- `void rescale_src(const double s)`
- `void rescale_dst(const double s)` -- change scale factors applied before
  and after conversions. If childs do not do the scaling they should redefine
  this.

- `virtual double frw_ang(dPoint p, double a, double dx) const`
- `virtual double bck_ang(dPoint p, double a, double dx) const` --
  Convert angle (radians, ccw from y=const) at point p.

- `virtual double frw_angd(dPoint p, double a, double dx) const`
  `virtual double bck_angd(dPoint p, double a, double dx) const` --
  Convert angle (degrees, ccw from y=const) at point p.

- `dPoint scales(const dRect & box) const;` --
  Linear scales, destination units per source units in x and y direction.
  box is given in source coordinates.


-----------------
## ConvMulti class

Composite point transformation, child of ConvBase.

Methods (&cnv is a pointer to a ConvBase class or its child, frw is
a boolean flag for direction of the transformation `true` means forward):
- `ConvMulti()` -- empty (trivial transformation),
- `ConvMulti(&cnv1, &cnv2, frw1, frw2)` -- Combine two transformations.
- `push_front(&cnv, frw)` -- Add a transformation to the beginning of the list.
- `push_back(&cnv, frw)`  -- Add a transformation to the end of the list.
- `simplify(box, N, err)` -- Try to substitude all transformation by a single ConvAff.
- `size()` -- Return number of transformations.
- `reset()` -- Reset to the trivial transformation.
-----------------
## ConvAff2D class

2D affine transformation, child of ConvBase.
Works only with `x` and `y` coordinates.

Methods (map is a std::map(dPoint,dPoint)):
 - `ConvAff2D()` -- constructor, trivial transformation,
 - `ConvAff2D(map)` -- build a transformation using the map (`map<dPoint,dPoint>`),
 - `reset()` -- reset to the trivial transformation,
 - `reset(map)` -- reset using the map,
 - `det()` -- forward conversion determinant,
 - `shift_src(p)` -- shift by vector `p` before the transformation,
 - `shift_dst(p)` -- shift by vector `p` after the transformation,
 - `rescale_src(kx,ky)` -- rescale `x` and `y` before thetransformation,
 - `rescale_dst(kx,ky)` -- rescale `x` and `y` after thetransformation,
 - `get_src_err()` -- get error in source coordinates
 - `get_dst_err()` -- get error in destination coordinates
