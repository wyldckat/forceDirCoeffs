forceDirCoeffs
==============

This repository provides a function object based on `forceCoeffs` from OpenFOAM(R), but with the added side forces. This is based on the instructions given [here](http://www.cfd-online.com/Forums/openfoam/92884-adding-side-force-forcecoeffs-c.html) by the forum member [drrbradford](http://www.cfd-online.com/Forums/members/drrbradford.html) at [CFD-Online](http://www.cfd-online.com).


Disclaimer and License
======================

This offering is not approved or endorsed by OpenCFD Limited, the producer of the OpenFOAM software and owner of the OPENFOAM® and OpenCFD® trade marks.

This is bound to the same license as OpenFOAM, namely GNU Public License v3. Quoting from OpenFOAM's license statement:

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM. If not, see <http://www.gnu.org/licenses/>.


How to get+build forceDirCoeffs
===============================

These instructions are for OpenFOAM 2.2.x:

For getting and building from git:
```
git clone git://github.com/wyldckat/forceDirCoeffs.git
cd forceDirCoeffs
git checkout of22x
wmake libso forceDirCoeffs
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/forceDirCoeffs/archive/of22x.zip" -O forceDirCoeffs.zip
unzip forceDirCoeffs.zip
cd forceDirCoeffs-of22x
wmake libso forceDirCoeffs
```

There is port for OpenFOAM 2.1.x as well! Simply replace on the instructions above `of22x` for `of21x`.


How to use it
=============

Ask more about it on this thread: http://www.cfd-online.com/Forums/openfoam/92884-adding-side-force-forcecoeffs-c.html

