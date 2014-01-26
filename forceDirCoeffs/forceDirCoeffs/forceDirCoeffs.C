/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
<2012-11-04> <Bruno Santos>: Adapted the changes indicated here: 
  http://www.cfd-online.com/Forums/openfoam/92884-adding-side-force-forcecoeffs-c.html
-------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "forceDirCoeffs.H"
#include "dictionary.H"
#include "Time.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(Foam::forceDirCoeffs, 0);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::forceDirCoeffs::forceDirCoeffs
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    forces(name, obr, dict, loadFromFiles),
    liftDir_(vector::zero),
    dragDir_(vector::zero),
    sfDir_(vector::zero),
    pitchAxis_(vector::zero),
    magUInf_(0.0),
    lRef_(0.0),
    Aref_(0.0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::forceDirCoeffs::~forceDirCoeffs()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::forceDirCoeffs::read(const dictionary& dict)
{
    if (active_)
    {
        forces::read(dict);

        // Directions for lift and drag forces, and pitch moment
        dict.lookup("liftDir") >> liftDir_;
        dict.lookup("dragDir") >> dragDir_;
        dict.lookup("sfDir") >> sfDir_;
        dict.lookup("pitchAxis") >> pitchAxis_;

        // Free stream velocity magnitude
        dict.lookup("magUInf") >> magUInf_;

        // Reference length and area scales
        dict.lookup("lRef") >> lRef_;
        dict.lookup("Aref") >> Aref_;
    }
}


void Foam::forceDirCoeffs::writeFileHeader()
{
    if (forcesFilePtr_.valid())
    {
        forcesFilePtr_()
            << "# Time" << tab << "Cd" << tab << "Cl" << tab << "Cy" << tab << "Cm" << endl;
    }
}


void Foam::forceDirCoeffs::execute()
{
    // Do nothing - only valid on write
}


void Foam::forceDirCoeffs::end()
{
    // Do nothing - only valid on write
}


void Foam::forceDirCoeffs::write()
{
    if (active_)
    {
        // Create the forces file if not already created
        makeFile();

        forcesMoments fm = forces::calcForcesMoment();

        scalar pDyn = 0.5*rhoRef_*magUInf_*magUInf_;

        vector totForce = fm.first().first() + fm.first().second();
        vector totMoment = fm.second().first() + fm.second().second();

        scalar liftForce = totForce & liftDir_;
        scalar dragForce = totForce & dragDir_;
        scalar sideForce = totForce & sfDir_;
        scalar pitchMoment = totMoment & pitchAxis_;

        scalar Cl = liftForce/(Aref_*pDyn);
        scalar Cd = dragForce/(Aref_*pDyn);
        scalar Cy = sideForce/(Aref_*pDyn);
        scalar Cm = pitchMoment/(Aref_*lRef_*pDyn);

        if (Pstream::master())
        {
            forcesFilePtr_()
                << obr_.time().value() << tab
                << Cd << tab << Cl << tab << Cy << tab << Cm << endl;

            if (log_)
            {
                Info<< "forceDirCoeffs output:" << nl
                    << "    Cd = " << Cd << nl
                    << "    Cl = " << Cl << nl
                    << "    Cy = " << Cy << nl
                    << "    Cm = " << Cm << nl
                    << "    Cl(f) = " << Cl/2.0 - Cm << nl
                    << "    Cl(r) = " << Cl/2.0 + Cm << nl
                    << endl;
            }
        }
    }
}


// ************************************************************************* //
