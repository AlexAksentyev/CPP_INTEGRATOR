#include <iostream>
#include <vector>
#include <boost/numeric/odeint.hpp>
#include <fstream>
#include <stdlib.h>
#include <random>
#include <iomanip>
#include <algorithm>


using namespace boost::numeric::odeint;

using Gauss = std::normal_distribution<double>;

/* The type of container used to hold the state vector */
class state_type {

  using Container =  std::vector<double> ;
  using size_type = Container::size_type;
  Container data_;

public:
  using iterator = Container::iterator;
  using const_iterator = Container::const_iterator;
  static const size_type VAR_NUM = 4;
  
  state_type() {}
  state_type(size_type n): data_(n) {};
  state_type(const state_type& other) : data_(other.data_) {}
  template<class InputIterator>
  state_type(InputIterator first, InputIterator last) : data_(first, last) {}

  iterator begin() {return data_.begin();}
  const_iterator begin() const {return data_.begin();}
  iterator end() {return data_.end();}
  const_iterator end() const {return data_.end();}

  size_type count() const {return data_.size()/VAR_NUM;}
  size_type size() const {return data_.size();}

  void resize(size_type n) {data_.resize(n);}
  void resize (size_type n, const double& val) {data_.resize(n, val);}

  const double& operator() (int state_id, int var_id) const {
    return *(begin() + (var_id + state_id * VAR_NUM));
  }

  double& operator() (int state_id, int var_id){
    return *(begin() + (var_id + state_id * VAR_NUM));
  }

  state_type& operator/=(const state_type & rhs){
    size_t n_lhs = this->size();
    size_t n_rhs = rhs.size();
    if (n_lhs != n_rhs)
      std::cout << "LHS size != RHS size" << std::endl;

    std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::divides<double>());

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& out, state_type& state){

    using namespace std;
    out << scientific << right << setprecision(4);
    for (int i=0; i<state.count(); i++){
      out << i;
      for(int j=0; j<VAR_NUM; j++){
	out << setw(15) <<  state(i, j);
      }
      out << endl;
    }
    
    return out;
  }

}; // class state_type

inline state_type operator/(state_type lhs, const state_type& rhs){
  lhs /= rhs;
  return lhs;
}

state_type abs(state_type state){
  std::for_each(state.begin(), state.end(), static_cast<double (*)(double)>(std::abs));
  return state;
}


namespace boost { namespace numeric { namespace odeint {
      template<>
      struct is_resizeable<state_type>
      {
	typedef boost::true_type type;
	static const bool value = type::value;
      };

    } } }

class state_algebra : public boost::numeric::odeint::range_algebra {
    public:
        template< typename S >
        static double norm_inf( const S &s ){
	  using std::max_element;
	  // using abs;
	  state_type abs_s = abs(s);
	  return  *max( abs_s.begin(), abs_s.end() );
        }
}; // class state_algebra

/* The rhs of x' = f(x) defined as a class */
class harm_osc {

    double m_gam;

public:
    harm_osc( double gam ) : m_gam(gam) { }

    void operator() ( const state_type &x , state_type &dxdt , const double /* t */ )
    {
      for(int i=0; i<x.count(); i++){
	dxdt(i, 0) = x(i, 1);
        dxdt(i, 1) = -x(i, 0) - m_gam*x(i, 1);
	dxdt(i, 2) = 1;
	dxdt(i, 3) = 1e-6;
      }
    }
};


struct logger
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;

    logger( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};


int main(int argc, char** argv) {
  using namespace std;
  typedef runge_kutta_dopri5< state_type, double, state_type, double, state_algebra > error_stepper_type;

  harm_osc ho(.15);

  default_random_engine generator;
  Gauss standard_gauss(0, .01);
  
  vector<double> v;
  int ens_n = atoi(argv[1]);
  for(int i=0; i<state_type::VAR_NUM*ens_n; i++)
    v.push_back(standard_gauss(generator));
  state_type x(v.begin(), v.end());
  vector<state_type> x_vec;
  vector<double> t_vec;

  double len = atof(argv[2]);

  cout << x << endl;

  size_t num_stp = integrate_adaptive( make_controlled< error_stepper_type >( 1.0e-10 , 1.0e-6 ) ,
				       ho , x , 0.0 , len, 0.01, logger(x_vec, t_vec) );

  // cout << num_stp << endl;

  // ofstream data;
  // data.open("/home/alexa/REPOS/CPP_INTEGRATOR/data/controlled_test.dat");
  // for (int i=0; i<num_stp; i++)
  //   data << t_vec[i] << "\t"
  // 	 << x_vec[i][0] << "\t"
  // 	 << x_vec[i][1] << ","
  // 	 << x_vec[i][2] << ","
  // 	 << x_vec[i][3] << endl;

  return 0;
}
