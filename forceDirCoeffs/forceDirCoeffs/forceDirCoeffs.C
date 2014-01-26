/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
<2014-01-26> <Bruno Santos>: Adapted the changes indicated here, for 2.2.x: 
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
#include "IOmanip.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
defineTypeNameAndDebug(forceDirCoeffs, 0);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::forceDirCoeffs::writeFileHeader(const label i)
{
    if (i == 0)
    {
        // force coeff data

        file(i)
            << "# liftDir   : " << liftDir_ << nl
            << "# dragDir   : " << dragDir_ << nl
            << "# pitchAxis : " << pitchAxis_ << nl
            << "# magUInf   : " << magUInf_ << nl
            << "# lRef      : " << lRef_ << nl
            << "# Aref      : " << Aref_ << nl
            << "# CofR      : " << coordSys_.origin() << nl
            << "# Time" << tab << "Cm" << tab << "Cy" << tab << "Cd" << tab << "Cl" << tab
            << "Cl(f)" << tab << "Cl(r)";
    }
    else if (i == 1)
    {
        // bin coeff data

        file(i)
            << "# bins      : " << nBin_ << nl
            << "# start     : " << binMin_ << nl
            << "# delta     : " << binDx_ << nl
            << "# direction : " << binDir_ << nl
            << "# Time";

        for (label j = 0; j < nBin_; j++)
        {
            const word jn('[' + Foam::name(j) + ']');

            file(i)
                << tab << "Cm" << jn << tab << "Cy" << jn << tab << "Cd" << jn << tab << "Cl" << jn;
        }
    }
    else
    {
        FatalErrorIn("void Foam::forces::writeFileHeader(const label)")
            << "Unhandled file index: " << i
            << abort(FatalError);
    }

    file(i)<< endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::forceDirCoeffs::forceDirCoeffs
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    forces(name, obr, dict, loadFromFiles, false),
    liftDir_(vector::zero),
    dragDir_(vector::zero),
    sfDir_(vector::zero),
    pitchAxis_(vector::zero),
    magUInf_(0.0),
    lRef_(0.0),
    Aref_(0.0)
{
    read(dict);

    Info<< endl;
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


void Foam::forceDirCoeffs::execute()
{
    // Do nothing - only valid on write
}


void Foam::forceDirCoeffs::end()
{
    // Do nothing - only valid on write
}


void Foam::forceDirCoeffs::timeSet()
{
    // Do nothing - only valid on write
}


void Foam::forceDirCoeffs::write()
{
    forces::calcForcesMoment();

    if (!active_)
    {
        return;
    }

    if (Pstream::master())
    {
        functionObjectFile::write();

        scalar pDyn = 0.5*rhoRef_*magUInf_*magUInf_;

        Field<vector> totForce(force_[0] + force_[1] + force_[2]);
        Field<vector> totMoment(moment_[0] + moment_[1] + moment_[2]);

        List<Field<scalar> > coeffs(4);
        coeffs[0].setSize(nBin_);
        coeffs[1].setSize(nBin_);
        coeffs[2].setSize(nBin_);
        coeffs[3].setSize(nBin_);

        // lift, drag and moment
        coeffs[0] = (totForce & liftDir_)/(Aref_*pDyn);
        coeffs[1] = (totForce & dragDir_)/(Aref_*pDyn);
        coeffs[2] = (totForce & sfDir_)/(Aref_*lRef_*pDyn);
        coeffs[3] = (totMoment & pitchAxis_)/(Aref_*lRef_*pDyn);

        scalar Cl = sum(coeffs[0]);
        scalar Cd = sum(coeffs[1]);
        scalar Cy = sum(coeffs[2]);
        scalar Cm = sum(coeffs[3]);

        scalar Clf = Cl/2.0 + Cm;
        scalar Clr = Cl/2.0 - Cm;

        file(0)
            << obr_.time().value() << tab
            << Cm << tab << Cy << tab << Cd << tab << Cl << tab << Clf << tab << Clr
            << endl;

        if (log_)
        {
            Info<< type() << " " << name_ << " output:" << nl
                << "    Cm    = " << Cm << nl
                << "    Cy    = " << Cy << nl
                << "    Cd    = " << Cd << nl
                << "    Cl    = " << Cl << nl
                << "    Cl(f) = " << Clf << nl
                << "    Cl(r) = " << Clr << endl;
        }

        if (nBin_ > 1)
        {
            if (binCumulative_)
            {
                for (label i = 1; i < coeffs[0].size(); i++)
                {
                    coeffs[0][i] += coeffs[0][i-1];
                    coeffs[1][i] += coeffs[1][i-1];
                    coeffs[2][i] += coeffs[2][i-1];
                    coeffs[3][i] += coeffs[3][i-1];
                }
            }

            file(1)<< obr_.time().value();

            forAll(coeffs[0], i)
            {
                file(1)
                    << tab << coeffs[3][i]
                    << tab << coeffs[2][i]
                    << tab << coeffs[1][i]
                    << tab << coeffs[0][i];
            }

            file(1) << endl;
        }

        if (log_)
        {
            Info<< endl;
        }
    }
}


// ************************************************************************* //
