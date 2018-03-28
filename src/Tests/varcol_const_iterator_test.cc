
#include <vector>
#include <iostream>



using VarCol = std::vector<double>;


void iterate_print(const VarCol& state){
  using namespace std;
  for (VarCol::const_iterator it=state.begin();
       it!=state.end();
       ++it){
    cout << *it << endl;
  }

}


int main(){


  VarCol x;
  for (int i=0; i<10; i++)
    x.push_back(i);

  iterate_print(x);

  return 0;
}
