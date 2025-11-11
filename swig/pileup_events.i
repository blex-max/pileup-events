%module pileup_events

%{
#include "bind.hpp"
%}

%include "std_string.i"  // for parameters to count_events
%include "std_vector.i"  // for result of count_events

/* tell SWIG how to convert std::vector<int> */
%include "std_vector.i"
namespace std {
  %template(IntVector) vector<int>;
}

/* wrap pileup-events */
%include "bind.hpp"
