// TODO:
// * finish rhs::StateList
// * add log controls for deciding whether to log intermediate state values,
//   i.e. when a data log is passed to Lattice::track_through(),
//   if record_intermediate == true, pass the log to Element::track_through()
//   (This looks rather a pythonic solution; in c++ should use polymorphism somehow...)
// * define operator<<

#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

#include "right_hand_side.h"

namespace data_log{

  const static int COL_WIDTH = 15;
  const static int VAL_PREC = 4;

  struct MetaData {
    size_t current_turn;
    std::string element_name;
    size_t element_id;

    MetaData() : current_turn(0), element_name("*"), element_id(0) {}
    MetaData(size_t turn, std::string name, size_t eid)
      : current_turn(turn), element_name(name), element_id(eid) {}
    void overwrite(size_t turn, std::string name, size_t eid){
      current_turn = turn;
      element_name = name;
      element_id = eid;
    }

    friend std::ostream& operator<<(std::ostream& out_stream, const MetaData& data){
      using namespace std;
      out_stream << setw(COL_WIDTH) << data.current_turn
		 << setw(COL_WIDTH) << data.element_name
		 << setw(COL_WIDTH) << data.element_id;
      return out_stream;
    }
    
  };

  class DataLog{
    std::vector<rhs::State> system_state_;
    std::vector<double> system_position_;
    // std::vector<std::string> element_name_;
    std::vector<MetaData> state_metadata_;
    int reference_pid_;

  public:

    DataLog() : system_state_(), system_position_(), state_metadata_(), reference_pid_(0){}
  
    void operator() (const rhs::State &state, double position);
    //  void operator() (const rhs::State &state, double position, std::string element_name);
    void operator() (const rhs::State &state, double position, const MetaData& );
  
    void set_reference(int pid){reference_pid_ = pid;}

    size_t size(){return system_state_.size();}
  
    void write_to_file(std::string name, std::string dir = "../data"); // binary is in build, go out
    friend std::ostream& operator<<(std::ostream& out_stream, const DataLog& data_log){
      using namespace std;
      int num_rows = data_log.system_position_.size();
      int num_states = data_log.system_state_[0].rows();
      // printing header 
      out_stream << setw(COL_WIDTH) << "#turn" // # for gnuplot comment
    		 << setw(COL_WIDTH) << "name"
    		 << setw(COL_WIDTH) << "EID"
    		 << setw(COL_WIDTH) << "s_loc"
    		 << setw(COL_WIDTH) << "PID";
      for (int j=0; j<rhs::VAR_NUM; j++) // variable names
    	out_stream << setw(COL_WIDTH) << rhs::VMAP.right.at(j);
      out_stream << endl;

      for (int state_i=0; state_i<num_rows; state_i++){ // current ensemble state
    	for(int pid=0; pid<num_states; pid++){ // particle pid
    	  out_stream << setw(COL_WIDTH) << data_log.state_metadata_[state_i]
    		     << setw(COL_WIDTH) << data_log.system_position_[state_i]
    		     << setw(COL_WIDTH) << pid;

    	  for (int var_j=0; var_j<rhs::VAR_NUM; var_j++) // state values
    	    out_stream << setw(COL_WIDTH) << data_log.system_state_[state_i](pid, var_j);

    	  out_stream << endl;
    	}
    	out_stream << "#\n";
      }
      return out_stream;
    }
    
    // var names accept format: FLAG var_name, where FLAG can be -D for
    // plotting the difference between the pid particle's var_name and the reference particle's var_name
    void plot(std::string var_y_name, std::string var_x_name, int pid, std::string line_type="points");
  };

  // TO FINISH
  // object for creating ensembles of initial conditions
  class StateList{};

} // namespace data_log


#endif // DATA_LOG_H
