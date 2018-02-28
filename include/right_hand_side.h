// The Right Hand Side of the differential equation
// corresponding to a particular element
// has a pointer to the element for the computation of
// the EM fields

// TODO:
// * (possibly) define IMAP as unordered_map
// * Consider moving from Eigen to boost::uBLAS for performance reasons


#ifndef RIGHT_HAND_SIDE_H
#define RIGHT_HAND_SIDE_H

#include <vector>
#include <Eigen/Dense>
#include <string>
#include <boost/bimap.hpp>


using imap_type = boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<int>>;
static const std::vector<imap_type::value_type>v{{"x",  0},  {"y",     1},  {"s",   2},
						 {"t",  3},  {"Theta", 4},  {"H",   5},
			                         {"px", 6},  {"py",    7},  {"dK",  8},
			                         {"Sx", 9},  {"Sy",   10},  {"Sz", 11}};
static const imap_type IMAP(v.begin(), v.end());

static const int VAR_NUM = 12;
static const std::string VAR_NAME[] = {"x", "y", "s",
				       "t", "Theta", "H",
				       "px", "py", "dK",
				       "Sx", "Sy", "Sz"};

// variable names
/*   x, y, s, */        // 0,  1,  2
/*   t, Theta, H, */    // 3,  4,  5
/*   px, py, dK, */     // 6,  7,  8
/*   Sx, Sy, Sz */      // 9,  10, 11

using State = Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM>;

class Particle;
class Element;
class DataLog;
class RightHandSide{
  Particle& particle_;
  Element& host_;
  double w_freq_; // for now initialized to 0
  // change when have Lattice and can insert an RF

public:
  RightHandSide(Particle& reference_particle, Element& host_element);
  RightHandSide(const RightHandSide& to_copy, Element& new_host);
  RightHandSide(const RightHandSide& to_copy);
  void operator() (const State &x , State &dxds, const double /* s*/);
};



#endif // RIGHT_HAND_SIDE_H
