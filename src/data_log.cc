// TODO:
// * output data for all pids (now only pid 0)


#include "data_log.h"
#include <iomanip>
#include <stdlib.h>
#include <iostream>


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
