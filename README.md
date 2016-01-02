forceDirCoeffs
==============

This repository provides a function object based on `forceCoeffs` from OpenFOAM(R), but with the added side forces. This is based on the instructions given [here](http://www.cfd-online.com/Forums/openfoam/92884-adding-side-force-forcecoeffs-c.html) by the forum member [drrbradford](http://www.cfd-online.com/Forums/members/drrbradford.html) at [CFD-Online](http://www.cfd-online.com).

This [git repository](https://github.com/wyldckat/forceDirCoeffs) was brought to you by Bruno Santos (wyldckat@github working at [blueCAPE Lda](http://www.bluecape.com.pt)).


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

These instructions are for OpenFOAM 2.1.x:

For getting and building from git:
```
git clone git://github.com/wyldckat/forceDirCoeffs.git
cd forceDirCoeffs
git checkout of21x
wmake libso forceDirCoeffs
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/forceDirCoeffs/archive/of21x.zip" -O forceDirCoeffs.zip
unzip forceDirCoeffs.zip
cd forceDirCoeffs-of21x
wmake libso forceDirCoeffs
```

There is port for OpenFOAM 2.2.x as well! Simply replace on the instructions above `of21x` for `of22x`.


How to use it
=============

The idea is that you should add the following line to the "controlDict" file:
```
libs (libforceDirCoeffs.so);
```

In case you already have a line that starts with "libs", then add the library name to the list, for example:
If you have this:
```
libs (
    libforces.so
);
```

It becomes this:
```
libs (
    libforces.so
    libforceDirCoeffs.so
);
```


For more, ask about it on this thread: http://www.cfd-online.com/Forums/openfoam/92884-adding-side-force-forcecoeffs-c.html

