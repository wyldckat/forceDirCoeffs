/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

Class
    Foam::forceDirCoeffs

Description
    Derived from the forces function object, creates a specialisation to
    calculate lift and drag forces.

SourceFiles
    forceDirCoeffs.C
    IOforceDirCoeffs.H

\*---------------------------------------------------------------------------*/

#ifndef forceDirCoeffs_H
#define forceDirCoeffs_H

#include "forces.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class forceDirCoeffs Declaration
\*---------------------------------------------------------------------------*/

class forceDirCoeffs
:
    public forces
{
    // Private data

        // Force coefficient geometry

            //- Lift
            vector liftDir_;

            //- Drag
            vector dragDir_;
            
            //- Side Force
            vector sfDir_;

            //- Pitch
            vector pitchAxis_;


        // Free-stream conditions

            //- Velocity magnitude
            scalar magUInf_;


        // Reference scales

            //- Length
            scalar lRef_;

            //- Area
            scalar Aref_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        forceDirCoeffs(const forceDirCoeffs&);

        //- Disallow default bitwise assignment
        void operator=(const forceDirCoeffs&);


protected:

    //- Output file header information
    virtual void writeFileHeader();


public:

    //- Runtime type information
    TypeName("forceDirCoeffs");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        forceDirCoeffs
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~forceDirCoeffs();


    // Member Functions

        //- Read the forces data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Write the forces
        virtual void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
