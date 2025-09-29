/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include <interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0; //Keep track of the current time in the simulation
    int context_save_time = 10 ; //Time taken to save context
    const int isr_body_time = 40;    //ISR body execution time
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            //Log CPU burst
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        } else if (activity == "SYSCALL") {
            //Run interrupt sequence for SYSCALL
            int device_num = duration_intr;

            //Interrupt boilerplate
            auto [boilerplate_execution, updated_time] = intr_boilerplate(current_time, device_num, context_save_time, vectors);
            execution += boilerplate_execution;
            current_time = updated_time;

            //ISR body
            execution += std::to_string(current_time) + ", "  + std::to_string(isr_body_time) + ", execute ISR body\n";
            current_time += isr_body_time;

            //IRET
            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time++;

            //Schedule device I/O completion
            int io_completion_time = current_time + delays[device_num];
            execution += std::to_string(io_completion_time) + ", " + std::to_string(delays[device_num]) + ", device " + std::to_string(device_num) + " completed I/O\n";

        } else if (activity == "END_IO") {
            //Handle end of I/O notification
            int device_num = duration_intr;
            execution += std::to_string(current_time) + ", 1, END_IO from device " + std::to_string(device_num) + "\n";
            current_time++;

        } else {
            // Error catching for unknown activity/input
            execution += "# Warning: unknown activity in trace\n";
        }
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
