// defined classes: Field, Tilt, Element
// Element contains the E-, B-field definitions.
// TODO:
//    * data output is performed only inside odeint,
//      but in python code I output values after odeint + rear_kick
//    * copy constructor

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <iomanip>

#include "Core/particle.h"
#include "Core/data_log.h"
#include "Core/right_hand_side.h"
#include "Core/tilt.h"

namespace integrator {
  namespace element{

    using VectorizedField = Eigen::Matrix<double, 3, Eigen::Dynamic>;

    struct ElementPars {
      double curve, length;
      std::string name;

      ElementPars(double curve, double length, std::string name="Element")
	: curve(curve), length(length), name(name) {}
    };

    struct UniFieldPars : public ElementPars{
      double field;

      UniFieldPars(double length, double field, std::string name)
	: ElementPars(0, length, name), field(field) {}
    };

    struct GradFieldPars : public ElementPars {
      double grad;

      GradFieldPars(double length, double grad, std::string name)
	: ElementPars(0, length, name), grad(grad) {}
    };

    class Element : boost::noncopyable{

      virtual Element* do_clone() const = 0;
      
      double curve_;
      double length_;
      std::string name_;
      rhs::RightHandSide rhs_;

    protected:
      Vector3d E_field_base_;
      Vector3d B_field_base_;
      VectorizedField E_field_vectorized_;
      VectorizedField E_field_prime_s_vectorized_;
      VectorizedField B_field_vectorized_;

      Element(const Element& ); // make elements non-copyable

    public:
      Element* clone() const {return do_clone();}

      Tilt tilt;

      void vectorize_fields(rhs::State state_matrix); // public for now, might move

      Element(Particle& particle,
	      double curve, double length,
	      std::string name="Element");

      Element(Particle& particle, ElementPars epars)
	: Element(particle, epars.curve, epars.length, epars.name) {}

      // Element(const Element& ); // copy constructor

      virtual bool is_RF() {return false;}

      double curve(){return curve_;}
      double length(){return length_;}
      std::string name(){return name_;}
      void rename(std::string new_name){name_ = new_name;}
      void set_RF_w_freq(double value){rhs_.set_w_freq(value);}

      void print_fields(); // for testing purposes
      void print_vectorized_fields(); // testing

      virtual VectorizedField EField(rhs::State state_matrix);
      virtual VectorizedField EField_prime_s(rhs::State state_matrix);
      virtual VectorizedField BField(rhs::State state_matrix);

      virtual void front_kick(rhs::State& state_matrix);
      virtual void rear_kick(rhs::State& state_matrix);

      void print();
      friend std::ostream& operator<<(std::ostream& out_stream, const Element& element){
	using namespace std;
	out_stream << right
		   << setw(10) << "name"
		   << setw(10) << "curvature"
		   << setw(10) << "length" << endl
		   << setw(10) << element.name_
		   << setw(10) << element.curve_
		   << setw(10) << element.length_;
	return out_stream;
      }

      // tracking with intermediate values output
      virtual size_t track_through(rhs::State& ini_states, data_log::DataLog& observer);
      // tracking w/o intermediate values output;
      // data logging is handled in the Lattice class' track_through
      virtual size_t track_through(rhs::State& ini_states);
    };

    inline Element* new_clone(const Element& e) {return e.clone();}

    class Observer : public Element {
    public:
      Observer(Particle& particle, std::string name="Observer")
	: Element(particle, 0, 0, name){}

      size_t track_through(rhs::State& ini_states, data_log::DataLog& observer) {
	observer(ini_states, ini_states(0, 2));
	return 0;
      }
    };
  } // element namespace
} //namespace integrator

#endif // ELEMENT_H
