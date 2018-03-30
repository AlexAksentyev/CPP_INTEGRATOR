
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

State::State(OldState old_state){
  size_type col_num = old_state.cols();
  if (col_num != State::VAR_NUM){
    std::cout << "Incorrect column count in matrix!" << std::endl;
    return;
  }

  state_num_ = old_state.rows();
  for(size_type row = 0; row<state_num_; row++)
    for(size_type col=0; col<col_num; col++)
      data_.push_back(old_state(row, col));
}

State State::from_config(const std::string path){
  OldState old_state = utilities::read_matrix<OldState>(path);
  std::cout << "old state made "<< std::endl;
  return State(old_state);
}

// int main(){
//   using namespace integrator;
//   using namespace std;
//   VariableCol x(5, -1);
//   VariableCol y(5, 2);

//   VariableCol z = x+y;

//   for (VariableCol::iterator it=z.begin(); it!=z.end(); ++it)
//     cout << *it << endl;

//   return 0;

// }
