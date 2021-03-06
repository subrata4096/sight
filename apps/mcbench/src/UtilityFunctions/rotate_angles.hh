//----------------------------------*-C++-*----------------------------------//
// Copyright 2009 Lawrence Livermore National Security, LLC
// All rights reserved.
//---------------------------------------------------------------------------//

// This work performed under the auspices of the U.S. Department of Energy by
// Lawrence Livermore National Laboratory under Contract DE-AC52-07NA27344

//  DISCLAIMER
//  This work was prepared as an account of work sponsored by an agency of the
//  United States Government. Neither the United States Government nor the
//  Lawrence Livermore National Security, LLC, nor any of their employees,
//  makes any warranty, express or implied, including the warranties of
//  merchantability and fitness for a particular purpose, or assumes any
//  legal liability or responsibility for the accuracy, completeness, or
//  usefulness of any information, apparatus, product, or process disclosed,
//  or represents that its use would not infringe privately owned rights.

#ifndef __rotate_angles_hh
#define __rotate_angles_hh

namespace IMC_namespace
{
//! Return a new angle with angles cos_theta_z_rel and phi relative to the pole.

template<typename Vector3d>
Vector3d rotate_angles( double cos_theta_z_rel, double phi,
                        const Vector3d& pole);

}    //    IMC_namespace

#endif // __rotate_angles_hh

