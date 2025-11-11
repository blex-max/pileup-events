%module pileup_events

%{
#include "bind.hpp"
%}

%include "std_vector.i"

/* tell SWIG how to convert std::vector<int> */
%include "std_vector.i"
namespace std {
  %template(IntVector) vector<int>;
}

/* wrap pileup-events */
%include "bind.hpp"
