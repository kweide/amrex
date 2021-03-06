/*
 *      .o.       ooo        ooooo ooooooooo.             ooooooo  ooooo 
 *     .888.      `88.       .888' `888   `Y88.            `8888    d8'  
 *    .8"888.      888b     d'888   888   .d88'  .ooooo.     Y888..8P    
 *   .8' `888.     8 Y88. .P  888   888ooo88P'  d88' `88b     `8888'     
 *  .88ooo8888.    8  `888'   888   888`88b.    888ooo888    .8PY888.    
 * .8'     `888.   8    Y     888   888  `88b.  888    .o   d8'  `888b   
 *o88o     o8888o o8o        o888o o888o  o888o `Y8bod8P' o888o  o88888o 
 *
 */

#ifndef _AGGSTENCILI_H_
#define _AGGSTENCILI_H_

namespace amrex
{
  /**************/
  template <class srcData_t, class dstData_t>
  AggStencil<srcData_t, dstData_t>::
  AggStencil(const std::vector<std::shared_ptr<BaseIndex>   > & a_dstVoFs,
             const std::vector<std::shared_ptr<BaseStencil> > & a_vofStencil,
             const srcData_t                                  & a_srcData,
             const dstData_t                                  & a_dstData)
  {
    BL_PROFILE("AggSten.constructor");
    m_ebstencil.resize(a_dstVoFs.size());
    m_dstAccess.resize(a_dstVoFs.size());

    for (int idst = 0; idst < a_dstVoFs.size(); idst++)
      {
        const BaseIndex& dstVoF = *a_dstVoFs[idst];
        m_dstAccess[idst].dataID = a_dstData.dataType(dstVoF);
        m_dstAccess[idst].offset = a_dstData.offset(dstVoF, 0);

        const BaseStencil& sten = *a_vofStencil[idst];
        m_ebstencil[idst].resize(sten.size());
        for (int isten = 0; isten < sten.size(); isten++)
          {
            const BaseIndex& stencilVoF = sten.index(isten);
            m_ebstencil[idst][isten].first.offset = a_srcData.offset(stencilVoF, sten.variable(isten));
            m_ebstencil[idst][isten].first.dataID = a_srcData.dataType(stencilVoF);
            m_ebstencil[idst][isten].second = sten.weight(isten);
          }
      }
    //  CH_STOP(t1);
  }
  /**************/
  template <class srcData_t, class dstData_t>
  void
  AggStencil<srcData_t, dstData_t>::
  apply(dstData_t       & a_lph,
        const srcData_t & a_phi,
        const int       & a_varDest,
        const bool      & a_incrementOnly) const
  {
    int varSrc = 0; int nComp = 1;
    apply(a_lph, a_phi, varSrc, a_varDest, nComp, a_incrementOnly);
  }
  /**************/
  template <class srcData_t, class dstData_t>
  void
  AggStencil<srcData_t, dstData_t>::
  apply(dstData_t       & a_lph,
        const srcData_t & a_phi,
        const int       & a_src,
        const int       & a_dst,
        const int       & a_nco,
        const bool      & a_incrementOnly) const
  {

    BL_PROFILE("AggSten::apply");
    const int numtypelph = a_lph.numDataTypes();
    const int numtypephi = a_phi.numDataTypes();
    std::vector<Real*>       dataPtrsLph(numtypelph);
    std::vector<const Real*> dataPtrsPhi(numtypephi);
    for (int icomp = 0; icomp < a_nco; icomp++)
      {
        int varDst = a_dst + icomp;
        int varSrc = a_src + icomp;
        for (int ivec = 0; ivec < numtypelph; ivec++)
          {
            dataPtrsLph[ivec] = a_lph.dataPtr(ivec, varDst);
          }

        for (int ivec = 0; ivec < numtypephi; ivec++)
          {
            dataPtrsPhi[ivec] = a_phi.dataPtr(ivec, varSrc);
          }

        for (int idst = 0; idst < m_ebstencil.size(); idst++)
          {
            Real* lphiPtr =  dataPtrsLph[m_dstAccess[idst].dataID] + m_dstAccess[idst].offset;
            Real& lphi = *lphiPtr;
            if (!a_incrementOnly)
              {
                lphi =  0.;
              }
            const stencil_t& ebstencil = m_ebstencil[idst];
            for (int isten = 0; isten < ebstencil.size(); isten++)
              {
                const Real& weight = ebstencil[isten].second;
                const long& offset = ebstencil[isten].first.offset;
                const int & dataID = ebstencil[isten].first.dataID;
                const Real& phiVal = *(dataPtrsPhi[dataID] + offset);
                lphi += phiVal*weight;
              }
          }
      }
  }

  template <class srcData_t, class dstData_t>
  void
  AggStencil<srcData_t, dstData_t>::
  cache(const dstData_t& a_lph) const
  {
    m_cacheDst.resize( m_ebstencil.size(), std::vector<Real>(a_lph.nComp(), 0.));
    BL_PROFILE("AggSten::cache");
    std::vector<const Real*> dataPtrsLph(a_lph.numDataTypes());
    for (int ivar = 0; ivar < a_lph.nComp(); ivar++)
      {
        for (int ivec = 0; ivec < dataPtrsLph.size(); ivec++)
          {
            dataPtrsLph[ivec] = a_lph.dataPtr(ivec, ivar);
          }

        for (int idst = 0; idst < m_ebstencil.size(); idst++)
          {
            const Real* lphPtr =  dataPtrsLph[m_dstAccess[idst].dataID] + m_dstAccess[idst].offset;
            m_cacheDst[idst][ivar] = *lphPtr;
          }
      }
  }
  /**************/
  template <class srcData_t, class dstData_t>
  void
  AggStencil<srcData_t, dstData_t>::
  uncache(dstData_t& a_lph) const
  {
    BL_PROFILE("AggSten::uncache");
    std::vector<Real*> dataPtrsLph(a_lph.numDataTypes());
    for (int ivar = 0; ivar < a_lph.nComp(); ivar++)
      {
        for (int ivec = 0; ivec < dataPtrsLph.size(); ivec++)
          {
            dataPtrsLph[ivec] = a_lph.dataPtr(ivec, ivar);
          }

        for (int idst = 0; idst < m_ebstencil.size(); idst++)
          {
            Real* lphPtr =  dataPtrsLph[m_dstAccess[idst].dataID] + m_dstAccess[idst].offset;
            *lphPtr = m_cacheDst[idst][ivar];
          }
      }
  }
  /**************/
}
#endif
