#include "Gadgetron.h"
#include "Gadget.h"
#include "GadgetReference.h"
#include "GadgetContainerMessage.h"
#include "hoNDArray.h"

#include <boost/python.hpp>
#include <numpy/arrayobject.h>

#include <complex>


GadgetReference::GadgetReference()
  : gadget_(0)
{
  //_import_array();
}

GadgetReference::~GadgetReference()
{

}

template<class T>
int GadgetReference::return_data(T header, boost::python::numeric::array arr)
{

  int ndims = PyArray_NDIM(arr.ptr());
  npy_intp* dims = PyArray_DIMS(arr.ptr());
  std::vector<unsigned int> dimensions(ndims);
  for (int i = 0; i < ndims; i++) dimensions[ndims-i-1] = static_cast<unsigned int>(dims[i]);

  GadgetContainerMessage< T >*         m1 = new GadgetContainerMessage< T >;
  memcpy(m1->getObjectPtr(), &header, sizeof(T));

  GadgetContainerMessage< hoNDArray< std::complex<float> > >* m2 = new GadgetContainerMessage< hoNDArray< std::complex<float> > >;
  m1->cont(m2);

  if (!m2->getObjectPtr()->create(&dimensions)) {
    GADGET_DEBUG1("Failed to create data storage for data returning from Python");
    return GADGET_FAIL;
    
  }

  memcpy(m2->getObjectPtr()->get_data_ptr(), PyArray_DATA(arr.ptr()), m2->getObjectPtr()->get_number_of_elements()*2*sizeof(float));

  if (gadget_) {
    //ACE_Time_Value wait = ACE_OS::gettimeofday() + ACE_Time_Value(0,1000); //1ms from now
    ACE_Time_Value nowait (ACE_OS::gettimeofday ());
    if (gadget_->next()->putq(m1,&nowait) == -1) {
      GADGET_DEBUG2("Putting message on Queue failed (%s)\n", gadget_->module()->name());
      GADGET_DEBUG2("Message Q: low mark %d, high mark %d, message bytes %d, message count %d\n",
		    gadget_->next()->msg_queue()->low_water_mark(), gadget_->next()->msg_queue()->high_water_mark(),
		    gadget_->next()->msg_queue()->message_bytes(),gadget_->next()->msg_queue()->message_count()); 
      return GADGET_FAIL;
    } else {
      return GADGET_OK;
    }
    //return gadget_->next()->putq(m1);
  } else {
    GADGET_DEBUG1("Data received from python, but no Gadget registered for output\n");
    m1->release();
    return GADGET_OK;
  }

  return GADGET_OK;

}

int GadgetReference::return_acquisition(GadgetMessageAcquisition acq, boost::python::numeric::array arr)
{
  return return_data<GadgetMessageAcquisition>(acq, arr);
}

int GadgetReference::return_image(GadgetMessageImage img, boost::python::numeric::array arr)
{
  return return_data<GadgetMessageImage>(img, arr);
}

