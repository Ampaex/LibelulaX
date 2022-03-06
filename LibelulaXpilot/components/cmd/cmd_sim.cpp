//FLIGHTGEAR SIMULATOR COMMANDS
#include "cmd_sim.hpp"

#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include "pidController.hpp"


/** Arguments of 'sim_flight_data' command */
static struct
{
    //struct arg_dbl *altitude;
    struct arg_dbl *pitch;
    //struct arg_dbl *roll;
    struct arg_end *end;
} sim_flight_data_args;

static int funcSimFlightData(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &sim_flight_data_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, sim_flight_data_args.end, argv[0]);
        return 1;
    }
    //ESP_LOGI("SIM_DATA", "Alt %lf / Pitch %lf / Roll %lf", sim_flight_data_args.altitude->dval[0], sim_flight_data_args.pitch->dval[0], sim_flight_data_args.roll->dval[0]);
    PIDdata pidata(ELEVATOR, sim_flight_data_args.pitch->dval[0]);
    sendToPID(pidata);
    setPIDsetpoint({ELEVATOR, 8.});
    printf("%lf\n", getFromPID(ELEVATOR)._value);
    return 0;
}

void register_sim(void)
{
    //sim_flight_data_args.altitude = arg_dbl1(NULL, "alt", "<double>", "Altitude");
    sim_flight_data_args.pitch = arg_dbl1(NULL, "pit", "<double>", "Pitch");
    //sim_flight_data_args.roll = arg_dbl1(NULL, "rol", "<double>", "Roll");
    sim_flight_data_args.end = arg_end(3);

    const esp_console_cmd_t sim_flight_data_cmd = {
        .command = "sim_flight_data",
        .help = "Get simulation data from flightgear",
        .hint = NULL,
        .func = &funcSimFlightData,
        .argtable = &sim_flight_data_args};

    ESP_ERROR_CHECK(esp_console_cmd_register(&sim_flight_data_cmd));
}