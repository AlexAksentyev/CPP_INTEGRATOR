// TODO:
//     * revise spin handling in State constructor


#include <Core/state.h>

using namespace integrator;

VariableCol& VariableCol::operator+=(const VariableCol& rhs){
  size_t n_lhs = this->size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::plus<value_type>());

  return *this;
}

VariableCol& VariableCol::operator-=(const VariableCol& rhs){
  size_t n_lhs = this->size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::minus<value_type>());

  return *this;
}

VariableCol& VariableCol::operator*=(double factor){
  for (VariableCol::iterator it=this->begin();
       it!=this->end();
       ++it)
    *it *= factor;

  return *this;
}

VariableCol& VariableCol::operator*=(const VariableCol& rhs){
  size_t n_lhs = this->size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::multiplies<value_type>());

  return *this;
}

VariableCol& VariableCol::operator/=(double factor){
  for (VariableCol::iterator it=this->begin();
       it!=this->end();
       ++it)
    *it /= factor;

  return *this;
}

VariableCol& VariableCol::operator/=(const VariableCol& rhs){
  size_t n_lhs = this->size();
  size_t n_rhs = rhs.size();
  if (n_lhs != n_rhs)
    std::cout << "LHS size != RHS size" << std::endl;

  std::transform(this->begin(), this->end(), rhs.begin(), this->begin(), std::divides<value_type>());

  return *this;
}

VariableCol VariableCol::sqrt(VariableCol vec){
  for(VariableCol::iterator it=vec.begin();
      it!=vec.end();
      ++it)
    *it = std::sqrt(*it);

  std::for_each(vec.begin(), vec.end(), static_cast<double (*)(double)>(std::sqrt));

  return vec;
}

State::State(MatrixState old_state){
  size_type col_num = old_state.cols();
  if (col_num != State::VAR_NUM){
    std::cout << "Incorrect column count in matrix!" << std::endl;
    return;
  }

  state_num_ = old_state.rows();
  State::value_type Sx, Sy, Sz;
  for(size_type row = 0; row<state_num_; row++){
    for(size_type col=0; col<col_num; col++)
      data_.push_back(old_state(row, col));
    // save initial Sx, Sy, Sz for later use in spin correction
    Sx = old_state(row, col_num-3);
    Sy = old_state(row, col_num-2);
    Sz = old_state(row, col_num-1);
    // make sure that ||S0|| == 1
    if (std::sqrt(Sx*Sx + Sy*Sy + Sz*Sz) != 1)
      std::cout << "Unnormalized spin" << std::endl;
    // and specifically, ||Sxz|| == 1
    if(Sy != 0)
      std::cout << "Spin out of x-s plane" << std::endl;
    spin_0_.push_back(State::SpinTriple(Sx, Sy, Sz));
  }
}

State State::from_config(const std::string path){
  MatrixState old_state = utilities::read_matrix<MatrixState>(path);
  return State(old_state);
}

void State::correct_spin() {
  // rotation in the x-s plane is done in such a way, that
  // the spin of the reference particle is kept along its
  // original direction, i.e., we rotate the spin ensemble
  // by the angle by which Sxz_ref deviated from Sxz_0,
  // NOT the average angle

  // original Sx, Sz
  double Sx0 = spin_0_[0].get<0>();
  double Sz0 = spin_0_[0].get<2>();
  
  // current Sx, Sz
  double Sx = (*this)(0, 9), Sx_upd;
  double Sy;
  double Sz = (*this)(0, 11);
  double Sx2 = Sx*Sx;
  double Sz2 = Sz*Sz;

   /******************************************/
  // std::cout << "initial ref spin: "
  // 	    << "(" << Sx0 << ","
  // 	    << Sz0 << ")" << std::endl;
  //   std::cout << "current ref spin: "
  // 	    << "(" << Sx << ","
  // 	    << Sz << ")" << std::endl;
  /*******************************************/

  // computing the rotation matrix
  double norm_Sxz = std::sqrt(Sx2 + Sz2);
  // correct the norm in case (Sx, Sz) == (0,0)
  norm_Sxz = norm_Sxz != 0? norm_Sxz : 1;
  // sin, cos for the deviation angle
  double sin_phi = (Sz0*Sx - Sx0*Sz)/norm_Sxz; //don't divide by norm Sxz0 here
  double cos_psy = (Sx0*Sx + Sz0*Sz)/norm_Sxz; // because assume S0 in the x-z plane, and |S0| = 1
  // this is to rotate in the OPPOSITE direction
  int sgn = (cos_psy > 0) - (cos_psy < 0);
  sin_phi *= sgn;
  cos_psy *= sgn;

  /***********************************/
  // std::cout << "sin_phi: " << sin_phi << std::endl;
  // std::cout << "cos_psy: " << cos_psy << std::endl;
  /**********************************/

  // rotation and orthogonalization
  for(int i=0; i<count(); i++){
    // rotate the vector as is
    // copy the current values of Sx, Sz
    Sx = (*this)(i, 9);
    Sz = (*this)(i, 11);
    /****************************************/
    // std::cout << "(" << Sx << "," << Sz << ")" << std::endl;
    /***************************************/
    // rotate
    Sx_upd = cos_psy*Sx - sin_phi*Sz; // this split into 2 lines
    (*this)(i,  9) = Sx_upd; // to avoid a operator() call
    Sz = sin_phi*Sx + cos_psy*Sz; // we need to rotate Sz to check for its sign AFTER rotation

    /**********************/
    // std::cout << "(" << Sx_upd << "," << Sz << ")" << std::endl;
    // std::cout << "delta Sx: " << Sx_upd - Sx << std::endl;
    /**********************/

    // then for the updated Sx, Sz values.
    // the orthogonalization correction
    sgn = (Sz > 0) - (Sz < 0); // sign of the already ROTATED Sz
    Sy = (*this)(i, 10);
    Sx2 = Sx_upd*Sx_upd;
    Sz2 = 1 - Sx2 - Sy*Sy;
    (*this)(i, 11) = Sz2 > 0? std::sqrt(Sz2)*sgn : 0; 
  }
}
