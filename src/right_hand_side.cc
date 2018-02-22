// TODO:
// * add E_field_prime and update the energy formula (required? by the RF element)


#include "right_hand_side.h"
#include "particle.h"
#include "element.h"
#include <boost/numeric/odeint.hpp>

using namespace std;

RightHandSide::RightHandSide(Particle& reference, Element& host)
  : particle_(reference), host_(host), w_freq_(0) {}


void RightHandSide::operator() (const state_type &state,
				state_type &derivative,
				const double /* s*/){

  int state_num = state.rows();
  double kappa = host_.curve();
  variable_col I = variable_col::Ones(state_num);
  // variable_col x = state.col(0);
  variable_col hs = I + (variable_col)state.col(0)*kappa; // hs = 1 + kappa*x

  double P0c = particle_.Pc();
  variable_col Px = state.col(6)*P0c;
  variable_col Py = state.col(7)*P0c;

  variable_col dK = state.col(8);
  variable_col kin_energy = particle_.kinetic_energy(dK);
  variable_col Pc = particle_.Pc(dK);
  variable_col Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

  variable_col xp = Px*hs/Ps;
  variable_col yp = Py*hs/Ps;
  variable_col Hp = Pc*hs/Ps;

  variable_col beta = particle_.beta(dK);
  variable_col v = beta * CLIGHT;
  variable_col tp = Hp/v;

  vectorized_field_type E_field = host_.EField(state);
  vectorized_field_type E_field_prime_s = host_.EField_prime_s(state);
  vectorized_field_type B_field = host_.BField(state);

  variable_col Ex = E_field.row(0), Ey = E_field.row(1), Es = E_field.row(2);
  variable_col Esp = E_field_prime_s.row(2);
  variable_col Bx = B_field.row(0), By = B_field.row(1), Bs = B_field.row(2);

  variable_col s = state.col(2);
  variable_col dKp = (Ex*xp + Ey*yp + Es + Esp*s)*1e-6;
  // this formula is for static fields (cf Andrey's thesis p 25)
  // for dynamically varying fields (like RF) add the term:
  // Esp*s

  variable_col mass0 = particle_.mass0()*I; // particle rest mass in MeVs
  variable_col mass02 = mass0*mass0;
  variable_col tot_nrg2 = (mass0 + kin_energy)*(mass0+kin_energy);
  variable_col gammap = dKp/mass0;

  double m0_kg = particle_.mass0_kg();
  variable_col gamma = particle_.gamma(dK);
  // Andrey's thesis p 35 for the following formulas:
  variable_col betap = dKp*mass02/(tot_nrg2*sqrt(kin_energy*kin_energy + 2*kin_energy*mass0));
  variable_col D = EZERO/(m0_kg*hs) * (xp*By - yp*Bx + Hp*Es/v) - gamma*v/(Hp*hs)*3*kappa*xp;
  variable_col xpp = -Hp*D*xp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ex/v + yp*Bs - hs*By) + kappa*hs;
  variable_col ypp = -Hp*D*yp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ey/v + hs*Bx - xp*Bs);
  variable_col factor0 = betap/beta - gammap/gamma;
  variable_col factor1 = Px*xpp/(Pc*Hp) + Py*ypp/(Pc*Hp) + hs*kappa*xp/(Hp*Hp);
  variable_col Pxp = Px*factor0 + Pc*xpp/Hp - Px*factor1;
  variable_col Pyp = Py*factor1 + Pc*ypp/Hp - Py*factor1;

  // too much headache to convert TBMT to MeVs, so
  // convert momenta from MeVs to SI units
  double conv_ftr = EZERO*1e6/CLIGHT;
  Px = Px * conv_ftr;
  Py = Py * conv_ftr;
  Ps = Ps * conv_ftr;

  double G = particle_.G();
  double m0c2 = m0_kg*CLIGHT*CLIGHT;
  variable_col t6 = tp * (EZERO/(gamma*m0_kg*m0c2)) * (G + 1/(1 + gamma));
  variable_col sp1 = tp*(-EZERO / (gamma*m0_kg))*(1 + G * gamma);
  variable_col sp2 = tp*( EZERO / (gamma*m0_kg*m0_kg * m0c2)) *
    G/(1 + gamma) * (Px*Bx +Py*By +Ps*Bs);

  variable_col Sx = state.col(9);
  variable_col Sy = state.col(10);
  variable_col Sz = state.col(11);
  
  variable_col Sxp = kappa * Sz +
    t6 * ((Ps * Ex - Px * Es) * Sz  -
	  (Px * Ey - Py * Ex) * Sy) +
    (sp1*By+sp2*Py)*Sz -
    (sp1*Bs+sp2*Ps)*Sy;
  variable_col Syp = t6 * ((Px * Ey - Py * Ex) * Sx -
			   (Py * Es - Ps * Ey) * Sz) +
    (sp1*Bs+sp2*Ps)*Sx -
    (sp1*Bx+sp2*Px)*Sz;
  variable_col Szp = (-1)*kappa * Sx +
    t6 * ((Py * Es - Ps * Ey) * Sy -
	  (Ps * Ex - Px * Es) * Sx) +
    (sp1*Bx+sp2*Px)*Sy -
    (sp1*By+sp2*Py)*Sx;

  //setting derivatives
  derivative.col(0) = xp; derivative.col(1) = yp; derivative.col(2) = I;
  derivative.col(3) = tp; derivative.col(4) = w_freq_*tp; derivative.col(5) = Hp;
  derivative.col(6) = Pxp/P0c; derivative.col(7) = Pyp/P0c; derivative.col(8) = dK/particle_.kinetic_energy();
  derivative.col(9) = Sxp; derivative.col(10) = Syp; derivative.col(11) = Szp;
}
