// TODO:
// * add E_field_prime and update the energy formula (required? by the RF element)


#include "right_hand_side.h"
#include "particle.h"
#include "element.h"
#include <boost/numeric/odeint.hpp>

using namespace std;

RightHandSide::RightHandSide(Particle& reference, Element& host)
  : particle_(reference), host_(host), w_freq_(0) {}

RightHandSide::RightHandSide(const RightHandSide& to_copy, Element& new_host)
  : particle_(to_copy.particle_), host_(new_host), w_freq_(to_copy.w_freq_){
  cout << "Particle reference unchanged" << endl;
}

RightHandSide::RightHandSide(const RightHandSide& to_copy)
  : RightHandSide(to_copy, to_copy.host_) {
  cout << "Particle, Element references unchanged" << endl;
}


void RightHandSide::operator() (const State &state,
				State &derivative,
				const double /* s*/){

  int state_num = state.rows();
  double kappa = host_.curve();
  VariableCol I = VariableCol::Ones(state_num);
  // VariableCol x = state.col(0);
  VariableCol hs = I + (VariableCol)state.col(0)*kappa; // hs = 1 + kappa*x

  double P0c = particle_.Pc();
  VariableCol Px = state.col(6)*P0c;
  VariableCol Py = state.col(7)*P0c;

  VariableCol dK = state.col(8);
  VariableCol kin_energy = particle_.kinetic_energy(dK);
  VariableCol Pc = particle_.Pc(dK);
  VariableCol Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

  VariableCol xp = Px*hs/Ps;
  VariableCol yp = Py*hs/Ps;
  VariableCol Hp = Pc*hs/Ps;

  VariableCol beta = particle_.beta(dK);
  VariableCol v = beta * CLIGHT;
  VariableCol tp = Hp/v;

  cout << "here" << endl;

  VectorizedField E_field = host_.EField(state);
  cout << "E_field" << endl;
  VectorizedField E_field_prime_s = host_.EField_prime_s(state);
  cout << "E_prime" << endl;
  VectorizedField B_field = host_.BField(state);

  cout << "E_field.nrow: " << E_field.rows() << " ncol: " << E_field.cols() << endl;

  cout << "host name: " << host_.name() << endl;

  VariableCol Ex = E_field.row(0), Ey = E_field.row(1), Es = E_field.row(2);
  VariableCol Esp = E_field_prime_s.row(2);
  VariableCol Bx = B_field.row(0), By = B_field.row(1), Bs = B_field.row(2);

  VariableCol s = state.col(2);
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

  VariableCol Sx = state.col(9);
  VariableCol Sy = state.col(10);
  VariableCol Sz = state.col(11);
  
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

  cout << "deriv.nrow: " << derivative.rows() << endl;
  
  //setting derivatives
  derivative.col(0) = xp; derivative.col(1) = yp; derivative.col(2) = I;
  derivative.col(3) = tp; derivative.col(4) = w_freq_*tp; derivative.col(5) = Hp;
  derivative.col(6) = Pxp/P0c; derivative.col(7) = Pyp/P0c; derivative.col(8) = dK/particle_.kinetic_energy();
  derivative.col(9) = Sxp; derivative.col(10) = Syp; derivative.col(11) = Szp;
}
