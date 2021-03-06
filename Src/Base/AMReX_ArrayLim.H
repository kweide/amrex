
#ifndef BL_ARRAYLIM_H
#define BL_ARRAYLIM_H

#include <AMReX_REAL.H>

#if !defined(BL_LANG_FORT)

#include <array>

/*
** C++ stuff ...
*/

#if     BL_SPACEDIM==1
#define ARLIM_P(x)  const int&
#define ARLIM(x)  (x)[0]
#define ARLIM_3D(x) std::array<int,3>{(x)[0], 0, 0}.data()
#define ZFILL(x) std::array<amrex::Real,3>{(x)[0], 0., 0.}.data()
#define BCREC_3D(x) std::array<int,6>{(x)[0], 0, 0, (x)[1], 0, 0}.data()
#elif   BL_SPACEDIM==2
#define ARLIM_P(x)  const int&,const int&
#define ARLIM(x)  (x)[0],(x)[1]
#define ARLIM_3D(x) std::array<int,3>{(x)[0], (x)[1], 0}.data()
#define ZFILL(x) std::array<amrex::Real,3>{(x)[0], (x)[1], 0.}.data()
#define BCREC_3D(x) std::array<int,6>{(x)[0], (x)[1], 0, (x)[2], (x)[3], 0}.data()
#elif   BL_SPACEDIM==3
#define ARLIM_P(x)  const int&,const int&,const int&
#define ARLIM(x)  (x)[0],(x)[1],(x)[2]
#define ARLIM_3D(x) x
#define ZFILL(x) x
#define BCREC_3D(x) x
#endif

#define BL_TO_FORTRAN(x) (x).dataPtr(), ARLIM((x).loVect()), ARLIM((x).hiVect())
#define BL_TO_FORTRAN_N(x,n) (x).dataPtr(n), ARLIM((x).loVect()), ARLIM((x).hiVect())

#define BL_TO_FORTRAN_3D(x) (x).dataPtr(), ARLIM_3D((x).loVect()), ARLIM_3D((x).hiVect())
#define BL_TO_FORTRAN_N_3D(x,n) (x).dataPtr(n), ARLIM_3D((x).loVect()), ARLIM_3D((x).hiVect())

#else

/*
** Fortran stuff ...
*/

#if __STDC__==1 || defined(__INTEL_COMPILER)

#if (BL_SPACEDIM == 1)
#define DIMS(a) a##_l1, a##_h1
#define DIMDEC(a) a##_l1, a##_h1
#define DIMV(a) a##_l1:a##_h1
#define DIM1(a) a##_l1:a##_h1
#define ARG_L1(a) a##_l1
#define ARG_H1(a) a##_h1
#define DIMARG(a) a##(1)
#endif

#if (BL_SPACEDIM == 2)
#define DIMS(a) a##_l1, a##_l2, a##_h1, a##_h2
#define DIMDEC(a) a##_l1, a##_l2, a##_h1, a##_h2
#define DIMV(a) a##_l1:a##_h1, a##_l2:a##_h2
#define DIM1(a) a##_l1:a##_h1
#define DIM2(a) a##_l2:a##_h2
#define ARG_L1(a) a##_l1
#define ARG_L2(a) a##_l2
#define ARG_H1(a) a##_h1
#define ARG_H2(a) a##_h2
#define DIMARG(a) a##(1),a##(2)
#endif

#if (BL_SPACEDIM == 3)
#define DIMS(a) a##_l1, a##_l2, a##_l3, a##_h1, a##_h2, a##_h3
#define DIMDEC(a) a##_l1, a##_l2, a##_l3, a##_h1, a##_h2, a##_h3
#define DIMV(a) a##_l1:a##_h1, a##_l2:a##_h2, a##_l3:a##_h3
#define DIM1(a) a##_l1:a##_h1
#define DIM2(a) a##_l2:a##_h2
#define DIM3(a) a##_l3:a##_h3
#define DIM12(a) a##_l1:a##_h1, a##_l2:a##_h2
#define DIM23(a) a##_l2:a##_h2, a##_l3:a##_h3
#define DIM13(a) a##_l1:a##_h1, a##_l3:a##_h3
#define ARG_L1(a) a##_l1
#define ARG_L2(a) a##_l2
#define ARG_L3(a) a##_l3
#define ARG_H1(a) a##_h1
#define ARG_H2(a) a##_h2
#define ARG_H3(a) a##_h3
#define DIMARG(a) a##(1),a##(2),a##(3)
#endif

#else

#if (BL_SPACEDIM == 1)
#define DIMS(a) a/**/_l1, a/**/_h1
#define DIMDEC(a) a/**/_l1, a/**/_h1
#define DIMV(a) a/**/_l1:a/**/_h1
#define DIM1(a) a/**/_l1:a/**/_h1
#define ARG_L1(a) a/**/_l1
#define ARG_H1(a) a/**/_h1
#define DIMARG(a) a/**/(1)
#endif

#if (BL_SPACEDIM == 2)
#define DIMS(a) a/**/_l1, a/**/_l2, a/**/_h1, a/**/_h2
#define DIMDEC(a) a/**/_l1, a/**/_l2, a/**/_h1, a/**/_h2
#define DIMV(a) a/**/_l1:a/**/_h1, a/**/_l2:a/**/_h2
#define DIM1(a) a/**/_l1:a/**/_h1
#define DIM2(a) a/**/_l2:a/**/_h2
#define ARG_L1(a) a/**/_l1
#define ARG_L2(a) a/**/_l2
#define ARG_H1(a) a/**/_h1
#define ARG_H2(a) a/**/_h2
#define DIMARG(a) a/**/(1),a/**/(2)
#endif

#if (BL_SPACEDIM == 3)
#define DIMS(a) a/**/_l1, a/**/_l2, a/**/_l3, a/**/_h1, a/**/_h2, a/**/_h3
#define DIMDEC(a) a/**/_l1, a/**/_l2, a/**/_l3, a/**/_h1, a/**/_h2, a/**/_h3
#define DIMV(a) a/**/_l1:a/**/_h1, a/**/_l2:a/**/_h2, a/**/_l3:a/**/_h3
#define DIM1(a) a/**/_l1:a/**/_h1
#define DIM2(a) a/**/_l2:a/**/_h2
#define DIM3(a) a/**/_l3:a/**/_h3
#define DIM12(a) a/**/_l1:a/**/_h1, a/**/_l2:a/**/_h2
#define DIM23(a) a/**/_l2:a/**/_h2, a/**/_l3:a/**/_h3
#define DIM13(a) a/**/_l1:a/**/_h1, a/**/_l3:a/**/_h3
#define ARG_L1(a) a/**/_l1
#define ARG_L2(a) a/**/_l2
#define ARG_L3(a) a/**/_l3
#define ARG_H1(a) a/**/_h1
#define ARG_H2(a) a/**/_h2
#define ARG_H3(a) a/**/_h3
#define DIMARG(a) a/**/(1),a/**/(2),a/**/(3)
#endif

#endif /*__STDC__==1*/

#endif /*!defined(BL_LANG_FORT)*/

#endif /*BL_ARRAYLIM_H*/
