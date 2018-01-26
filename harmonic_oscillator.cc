#include <iostream>
#include <vector>

#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;


typedef std::vector< double > state_type;

const double gam = 0.15;

/* The rhs of x' = f(x) */
class RightHandSide{
  double friction_coef_;
	
public:
  RightHandSide(double friction_coef) : friction_coef_(friction_coef){}
  void operator() (const state_type &x , state_type &dxdt, const double /* t*/){
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - friction_coef_*x[1];
  }
};

void harmonic_oscillator( const state_type &x , state_type &dxdt , const double /* t */ )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}
//]

struct push_back_state_and_time
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;

    push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};

struct DataLog{
  vector<state_type>& system_state_;
  vector<double>& system_time_;
  
  DataLog(vector<state_type>& states, vector<double>& times)
  : system_state_(states), system_time_(times){}
  
  void operator() (const state_type &state_vec, double t){
    system_state_.push_back(state_vec);
    system_time_.push_back(t);
  }
};

int main(int /* argc */ , char** /* argv */ )
{
    //using namespace std;
    //using namespace boost::numeric::odeint;


    //[ state_initialization
    state_type x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;
    //]
    

    RightHandSide rhs(gam);

    //[ integrate_observ
    vector<state_type> x_vec;
    vector<double> times;

    cout << "okay\n";

    size_t steps = integrate( rhs ,
			      x , 0.0 , 10.0 , 0.1, 
			      DataLog(x_vec, times));

    cout << "okay2\n";

    /* output */
    for( size_t i=0; i<steps; i++ ){
      cout << i << "\n" ;
      cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
      
}
