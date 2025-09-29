/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    
    int current_time = 0;           // Track current simulation time
    bool initialized = false;       // Track if variables are initialized
    int data_x = 0;                 // Data read from input device
    int data_y = 0;                 // Calculated data
    bool program_running = true;    // Main program loop control
    
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        // Simulate the main program execution
        if (!initialized) {
            // initialize_variables(); //CPU burst (*1)
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", OS starts\n";
            current_time++;
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", OS reads program into memory\n";
            current_time++;
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", OS starts program execution\n";
            current_time++;
            initialized = true;
        }

        // Handle different activities from trace
        if (activity == "CPU") {
            // CPU burst - simulate calculate(x) operation
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU execution\n";
            current_time += duration_intr;
            
            // Simulate calculation and processing
            data_y = data_x * 2; // Simple calculation example
            
        } else if (activity == "SYSCALL") {
            // x = read_input(); //read 'x' from some input device (*2)
            // This represents system call for I/O operations
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", system call\n";
            current_time++;
            
            // Simulate system call processing
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", system call processing\n";
            current_time += duration_intr;
            
            // Simulate reading input
            data_x++; // Increment as example of reading new data
            
        } else if (activity == "END_IO") {
            // print_output(y); //Send 'y' to some output device
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", I/O operation complete\n";
            current_time++;
            
            // Simulate I/O completion processing
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", process I/O completion\n";
            current_time += duration_intr;
            
            // delete(y, x); //CPU burst; free memory (cleanup)
            // This would be handled as part of the processing
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
