// wrapper around std::vector<double> with utilities for
// retrieving  

#ifndef	STATE_H
#define STATE_H

#include <boost/bimap.hpp>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <Eigen/Dense>
#include <Utilities/read_matrix.h>

#include <boost/numeric/odeint.hpp> // required for is_resizeable


namespace integrator {

  class VariableCol {

    using value_type = double;
    using data_container = std::vector<value_type>;
    data_container data_;
    
  public:

    using iterator = data_container::iterator;
    using const_iterator = data_container::const_iterator;
    using size_type = data_container::size_type;


    VariableCol() {}
    VariableCol(size_type n, const value_type& val) : data_(n, val){}
    VariableCol(const VariableCol& other) : data_(other.data_) {}
    
    template <class InputIterator>
    VariableCol(InputIterator first, InputIterator last)
      : data_(first, last){}
    VariableCol(Eigen::ArrayXd e_array)
      : VariableCol(e_array.data(), e_array.data() + e_array.size()) {}

    size_type size() const {return data_.size();}
    // void push_back(value_type val){data_.push_back(val);}

    iterator begin(){return data_.begin();}
    iterator end(){return data_.end();}
    const_iterator begin() const {return data_.begin();}
    const_iterator end() const {return data_.end();}

    static VariableCol sqrt(VariableCol vec);

    VariableCol& operator+=(const VariableCol& rhs);    
    VariableCol& operator-=(const VariableCol& rhs);
    VariableCol& operator*=(double factor);
    VariableCol& operator*=(const VariableCol& rhs);
    VariableCol& operator/=(double factor);
    VariableCol& operator/=(const VariableCol& rhs);
    value_type& operator[](int index) {return data_[index];}
    const value_type& operator[](int index) const {return data_[index];}
    
    friend std::ostream& operator<<(std::ostream& out_stream, VariableCol& vec){
      for (VariableCol::iterator it=vec.begin();
	   it!=vec.end();
	   ++it)
	out_stream << *it << std::endl;
      return out_stream;
    }
  };

  inline VariableCol operator+(VariableCol lhs, const VariableCol& rhs){
    lhs += rhs;
    return lhs;
  }
  inline VariableCol operator-(VariableCol lhs, const VariableCol& rhs){
    lhs -= rhs;
    return lhs;
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
  inline VariableCol operator*(VariableCol lhs, const VariableCol& rhs){
    lhs *= rhs;
    return lhs;
  }
  inline VariableCol operator/(const VariableCol& vec, double factor){
    VariableCol res(vec);
    res /= factor;
    return res;
  }
  inline VariableCol operator/(VariableCol lhs, const VariableCol& rhs){
    lhs /= rhs;
    return lhs;
  }
    
  using imap_type = boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<int>>;
  static const std::vector<imap_type::value_type>v{{"x",  0},  {"y",     1},  {"s",   2},
  						   {"t",  3},  {"Theta", 4},  {"H",   5},
  						   {"px", 6},  {"py",    7},  {"dK",  8},
  						   {"Sx", 9},  {"Sy",   10},  {"Sz", 11}};
  static const imap_type VMAP(v.begin(), v.end());

  // static const int VAR_NUM = 12;

  class State {
    using value_type = double;
    using data_container = std::vector<value_type>;
    using size_type = data_container::size_type;
    using OldState = Eigen::Matrix<double, Eigen::Dynamic, 12>; // *!

    data_container data_;
    size_type state_num_;
    
  public:
    const static size_type VAR_NUM = 12; // *!
    
    using iterator = data_container::iterator;
    using const_iterator = data_container::const_iterator;

    State(): state_num_(0) {}
    State(OldState old_state);
    static State from_config(const std::string path);
    State(const State& other): state_num_(other.state_num_), data_(other.data_) {}

    iterator begin(){return data_.begin();}
    iterator end(){return data_.end();}
    const_iterator begin() const {return data_.begin();}
    const_iterator end() const {return data_.end();}

    void resize(size_type n) {
      data_.resize(n);
      state_num_ = n/VAR_NUM;
    }
    void resize (size_type n, const value_type& val) {
      data_.resize(n, val);
      state_num_ = n/VAR_NUM;
    }

    size_type count() const {return state_num_;}
    size_type size() const {return data_.size();}

    bool has_NaN(){
      State::iterator it = std::find_if(this->begin(), this->end(), [](double d) { return std::isnan(d); });
      return !(it == this->end());
    }

    VariableCol var(int var_id) const { 
      VariableCol res(count(), 0); 
      size_t i, j;
      for(i=var_id, j=0; i<data_.size(); i += VAR_NUM, j++)
  	res[j] = *(this->begin()+i);
      
      return res;
    }
  
    VariableCol var(std::string name) const {
      int vid0 = VMAP.left.at(name);
      return var(vid0);
    }

    void set(int var_id, VariableCol data){
      if (count() != data.size())
  	std::cout << "Inconsistent sizes!" << std::endl;
      size_t i, j;
      for(i=var_id, j=0; i<data_.size(); i += VAR_NUM, j++)
  	data_[i] = data[j];
    }

    void set(std::string name, VariableCol data){
      int v_id = VMAP.left.at(name);
      set(v_id, data);
    }

    const value_type& operator()(int state_id, int var_id) const {
      return *(data_.begin() + (var_id + state_id*VAR_NUM));
    }
    value_type& operator()(int state_id, int var_id){
      return *(data_.begin() + (var_id + state_id*State::VAR_NUM));
    }

    value_type& operator[](int index) {return data_[index];}
    const value_type& operator[](int index) const {return data_[index];}

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
  }; // class State
} // namespace integrator

namespace boost { namespace numeric { namespace odeint {
      template<>
      struct is_resizeable<integrator::State>
      {
	typedef boost::true_type type;
	static const bool value = type::value;
      };

    } } }

#endif //STATE_H
