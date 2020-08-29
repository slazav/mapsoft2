## Mapsoft2

Documentation: http://slazav.github.io/mapsoft2

Old mapsoft: https://github.com/ushakov/mapsoft

## Building

Main part of the project is source code library
https://github.com/slazav/mapsoft2-libs attached
as git submodule.

To download everything use:
```
# git clone https://github.com/slazav/mapsoft2

# cd mapsoft2

# git submodule init

# git submodule update
```

It could be that latest state on the submodule is not commited to the
`mapsoft2` repository. Then one can try to do

```
# cd modules

# git checkout master
```

There is only one development branch, `master`. More or less stable
versions are build for Altlinux, see
https://packages.altlinux.org/ru/sisyphus/srpms/mapsoft2 They are marked
with git tags.

There is `modules.tar` archive in mapsoft2 repository. It is copy of
modules source code. It is used only for Altlinux builds and is updated
only for these builds. If you do not use `gear` program you do not need
this file.

Building is done by `make` program. In any directory you can run `make`
and it should build this directory together with all needed dependencies.

