#pragma once

#include <boost/smart_ptr.hpp>
#include <string>
#include <iostream>

template <class ARRAY_TYPE> class solver
{
 public:

  enum solverOutputModes { OUTPUT_SILENT = 0, OUTPUT_WARNINGS = 1, OUTPUT_VERBOSE = 2, OUTPUT_MAX = 3 };

  solver( int output_mode = OUTPUT_SILENT ) { output_mode_ = output_mode; }
  virtual ~solver() {}
  
  virtual void solver_error( std::string err ) { std::cerr << err << std::endl; }

  virtual void set_output_mode( int output_mode ) {
    if( !(output_mode >= OUTPUT_MAX || output_mode < 0 )) {
      output_mode_ = output_mode;
    }
  }

  virtual boost::shared_ptr<ARRAY_TYPE> solve( ARRAY_TYPE* ) = 0;

  void* operator new (size_t bytes) { return ::new char[bytes]; }
  void operator delete (void *ptr) { delete [] static_cast <char *> (ptr); } 
  void * operator new(size_t s, void * p) { return p; }

protected:
  int output_mode_;
};