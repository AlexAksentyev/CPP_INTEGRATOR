// TODO:
// * output data for all pids (now only pid 0)
// * plot multiple variables for the same pid
// * plot multiple pids for the same variable


#include "data_log.h"

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <boost/algorithm/string.hpp>

#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

#include "right_hand_side.h"

using namespace std;
using namespace data_log;
using namespace rhs;

void DataLog::operator() (const State &state, double position){
  system_state_.push_back(state);
  system_position_.push_back(position);
  state_metadata_.push_back(MetaData());
}

void DataLog::operator() (const State &state, double position, const MetaData& metadata){
  system_state_.push_back(state);
  system_position_.push_back(position);
  state_metadata_.push_back(metadata);
}

void DataLog::write_to_file(string name, string dir){
  int num_rows = system_position_.size();
  
  ofstream file_handle;
  file_handle.open((dir+"/"+name+".dat").c_str());

  file_handle << scientific << setprecision(VAL_PREC);
  
  file_handle << right
	      << setw(COL_WIDTH) << "#turn" // # for gnuplot comment
	      << setw(COL_WIDTH) << "name"
	      << setw(COL_WIDTH) << "EID"
	      << setw(COL_WIDTH) << "s_loc"; 
  for (int j=0; j<VAR_NUM; j++)
    file_handle << setw(COL_WIDTH) << VMAP.right.at(j);

  file_handle << endl;
  for (int i=0; i<num_rows; i++){
    file_handle << right
		<< setw(COL_WIDTH) << state_metadata_[i]
		<< setw(COL_WIDTH) << system_position_[i];

    for (int j=0; j<VAR_NUM; j++)
      file_handle << setw(COL_WIDTH) << system_state_[i](0,j);

    file_handle << endl;
  }

  file_handle.close();
}

void DataLog::plot(string var_y_name, string var_x_name, int pid, std::string line_type){
  string delim = " ";
  size_t d_pos_x = var_x_name.find(delim); // find the first occurence of blank space
  size_t d_pos_y = var_y_name.find(delim); // in flag_var_name format -FLAG var_name
  string x_flag = d_pos_x != string::npos? var_x_name.substr(0, d_pos_x) : " ";
  string y_flag = d_pos_y != string::npos? var_y_name.substr(0, d_pos_y) : " ";

  boost::to_upper(x_flag);
  boost::to_upper(y_flag);

  var_x_name.erase(0, d_pos_x+1); // remove flag
  var_y_name.erase(0, d_pos_y+1);
  int y_ind = VMAP.left.at(var_y_name), x_ind = VMAP.left.at(var_x_name);

  vector<double> x_vals, y_vals;
  double x_val, y_val, x_ref_val = 0, y_ref_val = 0;
  for(vector<State>::iterator it = system_state_.begin(); it != system_state_.end(); ++it){
    x_val = (*it)(pid, x_ind);
    y_val = (*it)(pid, y_ind);
    if (x_flag == "-D")
      x_ref_val = (*it)(reference_pid_, x_ind);
    if (y_flag == "-D")
      y_ref_val = (*it)(reference_pid_, y_ind);
    if(std::isnan(x_val) || std::isnan(y_val))
      continue;
    x_vals.push_back(x_val-x_ref_val);
    y_vals.push_back(y_val-y_ref_val);
  }

  Gnuplot gp;

  gp << "set xlabel '" << x_flag + " " + var_x_name << "'\n";
  gp << "set ylabel '" << y_flag + " " + var_y_name << "'\n";
  gp << "plot '-' with " << line_type << " title '" + var_y_name + "'\n";
  gp.send1d(boost::make_tuple(x_vals, y_vals));
 
}
