#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <boost/bimap.hpp>
#include <Eigen/Dense>
#include <Utilities/read_matrix.h>

typedef std::vector<double> VariableCol;

VariableCol& operator+=(VariableCol& lhs, const VariableCol& rhs){
  size_t n_lhs = lhs.size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  for (size_t i=0; i< n_lhs; i++)
    lhs[i] += rhs[i];

  return lhs;
}
inline VariableCol operator+(VariableCol lhs, const VariableCol& rhs){
  lhs += rhs;
  return lhs;
}

VariableCol& operator-=(VariableCol& lhs, const VariableCol& rhs){
  size_t n_lhs = lhs.size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  for (size_t i=0; i< n_lhs; i++)
    lhs[i] -= rhs[i];

  return lhs;
}
inline VariableCol operator-(VariableCol lhs, const VariableCol& rhs){
  lhs -= rhs;
  return lhs;
}

VariableCol& operator*=(VariableCol& vec, double factor){
  for (VariableCol::iterator it=vec.begin();
       it!=vec.end();
       ++it)
    *it *= factor;

  return vec;
}

inline VariableCol operator*(double factor, const VariableCol& vec){
  VariableCol res(vec);
  res *= factor;
  return res;
}
inline VariableCol operator*(const VariableCol& vec, double factor){
  VariableCol res(vec);
  res *= factor;
  return res;
}

VariableCol& operator/=(VariableCol& vec, double factor){
  for (VariableCol::iterator it=vec.begin();
       it!=vec.end();
       ++it)
    *it /= factor;

  return vec;
}

inline VariableCol operator/(const VariableCol& vec, double factor){
  VariableCol res(vec);
  res /= factor;
  return res;
}

VariableCol& operator/=(VariableCol& lhs, const VariableCol& rhs){
  size_t n_lhs = lhs.size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  for (size_t i=0; i< n_lhs; i++)
    lhs[i] /= rhs[i];

  return lhs;
}

inline VariableCol operator/(VariableCol lhs, const VariableCol& rhs){
  lhs /= rhs;
  return lhs;
}

VariableCol& operator*=(VariableCol& lhs, const VariableCol& rhs){
  size_t n_lhs = lhs.size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  for (size_t i=0; i< n_lhs; i++)
    lhs[i] *= rhs[i];

  return lhs;
}

inline VariableCol operator*(VariableCol lhs, const VariableCol& rhs){
  lhs *= rhs;
  return lhs;
}

VariableCol sqrt(VariableCol vec){
  for(VariableCol::iterator it=vec.begin();
      it!=vec.end();
      ++it)
    *it = sqrt(*it);

  return vec;
}

std::ostream& operator<<(std::ostream& out_stream, VariableCol& vec){
  for (VariableCol::iterator it=vec.begin();
       it!=vec.end();
       ++it)
    out_stream << *it << std::endl;

  return out_stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////

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
  
  VariableCol var(std::string name){
    VariableCol res(size()/VAR_NUM);
    int vid0 = VMAP.left.at(name);
    size_t i, j;
    for(i=vid0, j=0; i<size(); i += VAR_NUM, j++)
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

/////////////////////////////////////////////////////////////////////////////////////////////

class VectorizedField {

  VariableCol Fx_, Fy_, Fz_;
  size_t num_;

public:

  VectorizedField(){}

  VectorizedField(double base_vec[3], size_t rep_num)
    : num_(rep_num),
      Fx_(rep_num, base_vec[0]),
      Fy_(rep_num, base_vec[1]),
      Fz_(rep_num, base_vec[2]) {}
  
  VectorizedField(const VectorizedField& other)
    : num_(other.num_),
      Fx_(other.Fx_), Fy_(other.Fy_), Fz_(other.Fz_) {}
  
  void set(double base_vec[3], size_t rep_num){
    num_ = rep_num;
    Fx_.resize(rep_num, base_vec[0]);
    Fy_.resize(rep_num, base_vec[1]);
    Fz_.resize(rep_num, base_vec[2]);
  }

  static VectorizedField Zero(size_t rep_num){
    double zero[3]{0,0,0};
    return VectorizedField(zero, rep_num);
  }

  VariableCol x(){return Fx_;}
  VariableCol y(){return Fy_;}
  VariableCol z(){return Fz_;}

  friend std::ostream& operator<<(std::ostream& out_stream, const VectorizedField& field){
    out_stream << std::fixed;
    for(size_t i=0; i<field.num_; i++)
      out_stream << "(" << field.Fx_[i] << ", "
		        << field.Fy_[i] << ", "
		        << field.Fz_[i] << ")";
    return out_stream;
  }
};

int main() {

  using namespace std;

  double E[3]{3, 5, 1};

  VectorizedField Ev; Ev.set(E, 4);
  VectorizedField Bv = VectorizedField::Zero(5);

  cout << Bv << endl;

  VariableCol y = Ev.y();

  cout << Ev << endl;

  
}
