// TODO:
// * add E_field_prime and update the energy formula (required? by the RF element)


#include "Core/right_hand_side.h"
#include "Core/particle.h"
#include "Core/elements/element.h"
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace integrator::rhs;
using namespace integrator::element;

RightHandSide::RightHandSide(Particle& reference, Element& host)
  : particle_(reference), host_(host), w_freq_(0) {}

RightHandSide::RightHandSide(const RightHandSide& to_copy, Element& new_host)
  : particle_(to_copy.particle_), host_(new_host), w_freq_(to_copy.w_freq_){
  //  cout << "Particle reference unchanged" << endl;
}

RightHandSide::RightHandSide(const RightHandSide& to_copy)
  : RightHandSide(to_copy, to_copy.host_) {
  //  cout << "Particle, Element references unchanged" << endl;
}


void RightHandSide::operator() (const State &state,
				State &derivative,
				const double /* s*/){

  int state_num = state.count();
  double kappa = host_.curve();
  VariableCol I = VariableCol(state_num, 1);
  VariableCol hs = I + state.var(0)*kappa; // hs = 1 + kappa*x

  double P0c = particle_.Pc();
  VariableCol Px = state.var("Px")*P0c;
  VariableCol Py = state.var(7)*P0c;

  VariableCol dK = state.var(8);
  VariableCol kin_energy = particle_.kinetic_energy(dK);
  VariableCol Pc = particle_.Pc(dK);
  VariableCol Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

  VariableCol xp = Px*hs/Ps;
  VariableCol yp = Py*hs/Ps;
  VariableCol Hp = Pc*hs/Ps;

  VariableCol beta = particle_.beta(dK);
  VariableCol v = beta * CLIGHT;
  VariableCol tp = Hp/v;

  VectorizedField E_field = host_.EField(state);
  VectorizedField E_field_prime_s = host_.EField_prime_s(state);
  VectorizedField B_field = host_.BField(state);

  VariableCol Ex = E_field.x(), Ey = E_field.y(), Es = E_field.z();
  VariableCol Esp = E_field_prime_s.z();
  VariableCol Bx = B_field.x(), By = B_field.y(), Bs = B_field.z();

  VariableCol s = state.var(2);
  VariableCol dKp = (Ex*xp + Ey*yp + Es + Esp*s)*1e-6;
  // this formula is for static fields (cf Andrey's thesis p 25)
  // for dynamically varying fields (like RF) add the term:
  // Esp*s

  VariableCol mass0 = particle_.mass0()*I; // particle rest mass in MeVs
  VariableCol mass02 = mass0*mass0;
  VariableCol tot_nrg2 = (mass0 + kin_energy)*(mass0+kin_energy);
  VariableCol gammap = dKp/mass0;

  double m0_kg = particle_.mass0_kg();
  VariableCol gamma = particle_.gamma(dK);
  // Andrey's thesis p 35 for the following formulas:
  VariableCol betap = dKp*mass02/(tot_nrg2*sqrt(kin_energy*kin_energy + 2*kin_energy*mass0));
  VariableCol D = EZERO/(m0_kg*hs) * (xp*By - yp*Bx + Hp*Es/v) - gamma*v/(Hp*hs)*3*kappa*xp;
  VariableCol xpp = -Hp*D*xp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ex/v + yp*Bs - hs*By) + kappa*hs;
  VariableCol ypp = -Hp*D*yp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ey/v + hs*Bx - xp*Bs);
  VariableCol factor0 = betap/beta - gammap/gamma;
  VariableCol factor1 = Px*xpp/(Pc*Hp) + Py*ypp/(Pc*Hp) + hs*kappa*xp/(Hp*Hp);
  VariableCol Pxp = Px*factor0 + Pc*xpp/Hp - Px*factor1;
  VariableCol Pyp = Py*factor1 + Pc*ypp/Hp - Py*factor1;

  // too much headache to convert TBMT to MeVs, so
  // convert momenta from MeVs to SI units
  double conv_ftr = EZERO*1e6/CLIGHT;
  Px = Px * conv_ftr;
  Py = Py * conv_ftr;
  Ps = Ps * conv_ftr;

  double G = particle_.G();
  double m0c2 = m0_kg*CLIGHT*CLIGHT;
  VariableCol t6 = tp * (EZERO/(gamma*m0_kg*m0c2)) * (G + 1/(1 + gamma));
  VariableCol sp1 = tp*(-EZERO / (gamma*m0_kg))*(1 + G * gamma);
  VariableCol sp2 = tp*( EZERO / (gamma*m0_kg*m0_kg * m0c2)) *
    G/(1 + gamma) * (Px*Bx +Py*By +Ps*Bs);

  VariableCol Sx = state.var(9);
  VariableCol Sy = state.var(10);
  VariableCol Sz = state.var(11);
  
  VariableCol Sxp = kappa * Sz +
    t6 * ((Ps * Ex - Px * Es) * Sz  -
	  (Px * Ey - Py * Ex) * Sy) +
    (sp1*By+sp2*Py)*Sz -
    (sp1*Bs+sp2*Ps)*Sy;
  VariableCol Syp = t6 * ((Px * Ey - Py * Ex) * Sx -
			   (Py * Es - Ps * Ey) * Sz) +
    (sp1*Bs+sp2*Ps)*Sx -
    (sp1*Bx+sp2*Px)*Sz;
  VariableCol Szp = (-1)*kappa * Sx +
    t6 * ((Py * Es - Ps * Ey) * Sy -
	  (Ps * Ex - Px * Es) * Sx) +
    (sp1*Bx+sp2*Px)*Sy -
    (sp1*By+sp2*Py)*Sx;
  
  //setting derivatives
  derivative.var(0) = xp; derivative.var(1) = yp; derivative.var(2) = I;
  derivative.var(3) = tp; derivative.var(4) = w_freq_*tp; derivative.var(5) = Hp;
  derivative.var(6) = Pxp/P0c; derivative.var(7) = Pyp/P0c; derivative.var(8) = dKp/particle_.kinetic_energy();
  derivative.var(9) = Sxp; derivative.var(10) = Syp; derivative.var(11) = Szp;
}
