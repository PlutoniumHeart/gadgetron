#pragma once

#include "senseOperator.h"
#include "cuNDArray_operators.h"
#include "cuNDArray_elemwise.h"
#include "cuNDArray_blas.h"
#include "vector_td.h"
#include "gpupmri_export.h"

namespace Gadgetron{

  template<class REAL, unsigned int D> class EXPORTGPUPMRI cuSenseOperator : public senseOperator<REAL, D, cuNDArray< complext<REAL> > >
  {
    
  public:
    
    cuSenseOperator() : senseOperator<REAL,D,cuNDArray< _complext> >() {}
    virtual ~cuSenseOperator() {}
    
    typedef complext<REAL> _complext;
    
    virtual void mult_M( cuNDArray<_complext> *in, cuNDArray<_complext> *out, bool accumulate = false ) = 0;
    virtual void mult_MH( cuNDArray<_complext> *in, cuNDArray<_complext> *out, bool accumulate = false ) = 0;
    
    virtual void mult_csm( cuNDArray<_complext> *in, cuNDArray< _complext> *out );
    virtual void mult_csm_conj_sum( cuNDArray<_complext> *in, cuNDArray<_complext> *out );    
  };
}