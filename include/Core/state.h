// wrapper around std::vector<double> with utilities for
// retrieving  

#ifndef	STATE_H
#define STATE_H

#include <boost/bimap.hpp>
#include <vector>
#include <iomanip>
#include <fstream>
#include <Eigen/Dense>
#include <Utilities/read_matrix.h>


namespace integrator {

  using imap_type = boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<int>>;
  static const std::vector<imap_type::value_type>v{{"x",  0},  {"y",     1},  {"s",   2},
						   {"t",  3},  {"Theta", 4},  {"H",   5},
						   {"px", 6},  {"py",    7},  {"dK",  8},
						   {"Sx", 9},  {"Sy",   10},  {"Sz", 11}};
  static const imap_type VMAP(v.begin(), v.end());

  static const int VAR_NUM = 12;

  class State : public std::vector<double> {
    typedef Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM> OldState;
    size_t row_num_;
    size_t col_num_;
  public:
    State(OldState old_state){
      row_num_ = old_state.rows();
      col_num_ = old_state.cols();
      for(size_t row = 0; row<row_num_; row++)
	for(size_t col=0; col<col_num_; col++)
	  this->push_back(old_state(row, col));
    }
    static State from_config(const std::string& path){
      typedef Eigen::Matrix<double, Eigen::Dynamic, VAR_NUM> OldState;
      OldState old_state = utilities::read_matrix<OldState>(path);
      return State(old_state);
    }

    size_t count(){return row_num_;}
  
    VariableCol var(std::string name){
      VariableCol res(size()/VAR_NUM);
      int vid0 = VMAP.left.at(name);
      size_t i, j;
      for(i=vid0, j=0; i<size(); i += VAR_NUM, j++)
	res[j] = ((*this)[i]);

      return res;
    }

    VariableCol var(int var_id){ 
      VariableCol res(size()/VAR_NUM);
      size_t i, j;
      for(i=var_id, j=0; i<size(); i += VAR_NUM, j++)
	res[j] = ((*this)[i]);

      return res;
    }

    friend std::ostream& operator<<(std::ostream& out_stream, State& state_vec){
      size_t counter = 0, pid=0;
      out_stream << std::fixed << pid;
      for(State::iterator it=state_vec.begin();
	  it!=state_vec.end();
	  ++it){
	out_stream << std::setw(10) << *it << " ";
	counter++;
	if(counter==VAR_NUM){
	  pid++;
	  out_stream << std::endl << pid;
	  counter=0;
	}
      }
      return out_stream;
    }
  };


} // namespace integrator

#endif //STATE_H
