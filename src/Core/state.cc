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
  std::cout << "old state made "<< std::endl;
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
  double Sx = (*this)(0, 9);
  double Sy;
  double Sz = (*this)(0, 11);
  double Sx2 = Sx*Sx;
  double Sz2 = Sz*Sz;

  double norm_Sxz = std::sqrt(Sx2 + Sz2);
  // correct the norm in case (Sx, Sz) == (0,0)
  norm_Sxz = norm_Sxz != 0? norm_Sxz : 1;
  // sin, cos for the deviation angle
  double sin_phi = (Sz0*Sx - Sx0*Sz)/norm_Sxz;
  double cos_psy = (Sx0*Sx + Sz0*Sz)/norm_Sxz;
  // this is to rotate in the OPPOSITE direction
  int sgn = (cos_psy > 0) - (cos_psy < 0);
  sin_phi *= sgn;
  cos_psy *= sgn;
  
  for(int i=0; i<count(); i++){
    Sx = (*this)(i, 9);
    Sy = (*this)(i, 10);
    Sz = (*this)(i, 11);
    sgn = (Sz > 0) - (Sz < 0);
    Sx2 = Sx*Sx;
    Sz2 = 1 - Sx2 - Sy*Sy;

    // rotate the vector as is
    (*this)(i,  9) = cos_psy*Sx - sin_phi*Sz;
    (*this)(i, 11) = sin_phi*Sx + cos_psy*Sz;
    // then the orthogonalization correction
    (*this)(i, 11) = Sz2 > 0? std::sqrt(Sz2)*sgn : 0; 
  }
}
