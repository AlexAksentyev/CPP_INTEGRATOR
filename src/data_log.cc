// TODO:
// * output data for all pids (now only pid 0)
// * plot multiple variables for the same pid
// * plot multiple pids for the same variable


#include "data_log.h"
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>
#include "right_hand_side.h"

using namespace std;

void DataLog::write_to_file(string name, string dir){

  int col_width = 10;
  int num_rows = system_position_.size();
  
  ofstream file_handle;
  file_handle.open((dir+"/"+name+".dat").c_str());

  file_handle << fixed << setprecision(4);
  
  file_handle << right
	      << setw(col_width) << "#s"; // # for gnuplot comment
  for (int j=0; j<VAR_NUM; j++)
    file_handle << setw(col_width) << VAR_NAME[j];

  file_handle << endl;
  for (int i=0; i<num_rows; i++){
    file_handle << right
		<< setw(col_width) << system_position_[i];

    for (int j=0; j<VAR_NUM; j++)
      file_handle << setw(col_width) << system_state_[i](0,j);

    file_handle << endl;
  }

  file_handle.close();
}


void DataLog::plot(string var_name, int pid, string line_type){

  int var_index = IMAP.left.at(var_name);

  vector<double> var_values;
  double value;
  for(vector<State>::iterator it = system_state_.begin(); it != system_state_.end(); ++it){
    value = (*it)(pid, var_index);
    var_values.push_back(value);
  }
  
  Gnuplot gp;

  gp << "plot '-' with " << line_type << " title '" + var_name + "'\n";
  gp.send1d(boost::make_tuple(system_position_, var_values));
  
}

void DataLog::plot(string var_y_name, string var_x_name, int pid, std::string line_type){

  int y_ind = IMAP.left.at(var_y_name), x_ind = IMAP.left.at(var_x_name);

  vector<double> x_vals, y_vals;
  double x_val, y_val;
  for(vector<State>::iterator it = system_state_.begin(); it != system_state_.end(); ++it){
    x_val = (*it)(pid, x_ind);
    y_val = (*it)(pid, y_ind);
    if(std::isnan(x_val) || std::isnan(y_val))
      continue;
    x_vals.push_back(x_val);
    y_vals.push_back(y_val);
  }

  cout << "x_vals size: " << x_vals.size() << endl;
  Gnuplot gp;

  gp << "plot '-' with " << line_type << " title '" + var_y_name + "'\n";
  gp.send1d(boost::make_tuple(x_vals, y_vals));
}
