// TODO:
// * add E_field_prime and update the energy formula (required? by the RF element)


#include "Core/right_hand_side.h"
#include "Core/particle.h"
#include "Core/elements/element.h"
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace integrator::core::rhs;
using namespace integrator::core::element;

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

  double hs, kappa = host_.curve();

  double conv_ftr = EZERO*1e6/CLIGHT;

  double G = particle_.G();
  double mass0 = particle_.mass0(); // particle rest mass in MeVs
  double mass02 = mass0*mass0;
  double m0_kg = particle_.mass0_kg();
  double m0c2 = m0_kg*CLIGHT*CLIGHT;

  VectorizedField E_field = host_.EField(state);
  VectorizedField E_field_prime_s = host_.EField_prime_s(state);
  VectorizedField B_field = host_.BField(state);

  double P0c = particle_.Pc(), Pc, Px, Py, Ps;
  double dK, kin_energy, beta, v;
  double tot_nrg2, gammap;
  double gamma, betap, D, factor0, factor1, factor2;
  double xpp, ypp;
  double Ex, Ey, Es, Esp;
  double Bx, By, Bs;
  double Sx, Sy, Sz;
  double xp, yp,
    tp, Hp,
    Pxp, Pyp, dKp,
    Sxp, Syp, Szp;
  double t6, sp1, sp2;
  for (int i=0; i<state.count(); i++){
    hs = 1 + kappa*state(i, 0); // hs = 1 + kappa*x

    Px = state(i, 6)*P0c;
    Py = state(i, 7)*P0c;

    dK = state(i, 8);
    kin_energy = particle_.kinetic_energy(dK);
    Pc = particle_.Pc(dK);
    Ps = sqrt(Pc*Pc - Px*Px - Py*Py);

    xp = Px*hs/Ps;
    yp = Py*hs/Ps;
    Hp = Pc*hs/Ps;

    beta = particle_.beta(dK);
    v = beta * CLIGHT;
    tp = Hp/v;

    Ex = E_field(0, i); Ey = E_field(1, i); Es = E_field(2, i);
    Esp = E_field_prime_s(2, i);
    Bx = B_field(0, i); By = B_field(1, i); Bs = B_field(2, i);
    dKp = (Ex*xp + Ey*yp + Es + Esp*state(i, 2))*1e-6;
    
    tot_nrg2 = (mass0 + kin_energy)*(mass0+kin_energy);
    gammap = dKp/mass0;

    gamma = particle_.gamma(dK);
    // Andrey's thesis p 35 for the following formulas:
    betap = dKp*mass02/(tot_nrg2*sqrt(kin_energy*kin_energy + 2*kin_energy*mass0));
    D = EZERO/(m0_kg*hs) * (xp*By - yp*Bx + Hp*Es/v) - gamma*v/(Hp*hs)*3*kappa*xp;
    xpp = -1*Hp*D*xp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ex/v + yp*Bs - hs*By) + kappa*hs;
    ypp = -1*Hp*D*yp/(gamma*v) + CLIGHT*Hp/(Pc*1e6) * (Hp*Ey/v + hs*Bx - xp*Bs);
    factor0 = betap/beta - gammap/gamma;
    factor1 = Px*xpp/(Pc*Hp) + Py*ypp/(Pc*Hp) + hs*kappa*xp/(Hp*Hp);
    Pxp = Px*factor0 + Pc*xpp/Hp - Px*factor1;
    Pyp = Py*factor1 + Pc*ypp/Hp - Py*factor1;

    Px = Px * conv_ftr;
    Py = Py * conv_ftr;
    Ps = Ps * conv_ftr;

    t6 = tp * (EZERO/(gamma*m0_kg*m0c2)) * (G + 1/(1 + gamma));
    sp1 = tp*(-EZERO / (gamma*m0_kg))*(1 + G * gamma);
    sp2 = tp*( EZERO / (gamma*m0_kg*m0_kg * m0c2)) * G/(1 + gamma) * (Px*Bx +Py*By +Ps*Bs);

    Sx = state(i, 9);
    Sy = state(i, 10);
    Sz = state(i, 11);
  
    Sxp = kappa * Sz +
      t6 * ((Ps * Ex - Px * Es) * Sz  -
	    (Px * Ey - Py * Ex) * Sy) +
      (sp1*By+sp2*Py)*Sz -
      (sp1*Bs+sp2*Ps)*Sy;
    Syp = t6 * ((Px * Ey - Py * Ex) * Sx -
		(Py * Es - Ps * Ey) * Sz) +
      (sp1*Bs+sp2*Ps)*Sx -
      (sp1*Bx+sp2*Px)*Sz;
    Szp = (-1)*kappa * Sx +
      t6 * ((Py * Es - Ps * Ey) * Sy -
	    (Ps * Ex - Px * Es) * Sx) +
      (sp1*Bx+sp2*Px)*Sy -
      (sp1*By+sp2*Py)*Sx;

    
    derivative(i, 0) = xp; derivative(i, 1) = yp; derivative(i, 2) = 1;
    derivative(i, 3) = tp; derivative(i, 4) = w_freq_*tp; derivative(i, 5) = Hp;
    derivative(i, 6) = Pxp/P0c; derivative(i, 7) = Pyp/P0c; derivative(i, 8) = dKp/particle_.kinetic_energy();
    derivative(i, 9) = Sxp; derivative(i, 10) = Syp; derivative(i, 11) = Szp;
  }
}
