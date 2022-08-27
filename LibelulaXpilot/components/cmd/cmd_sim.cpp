//FLIGHTGEAR SIMULATOR COMMANDS
#include "cmd_sim.hpp"

#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include "aircraft.hpp"
#include "noiseLib.hpp"

/** Arguments of 'sim_flight_data' command */
static struct
{
    struct arg_dbl *pitch;
    struct arg_dbl *roll;
    struct arg_dbl *yaw;
    struct arg_dbl *altitude;
    struct arg_dbl *longitude;
    struct arg_dbl *latitude;
    struct arg_dbl *compass;
    struct arg_end *end;
} sim_flight_data_args;

static int funcSimFlightData(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &sim_flight_data_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, sim_flight_data_args.end, argv[0]);
        return 1;
    }

    //Simulation noise
    #if defined(CONFIG_NOISE_ENABLE) && defined(CONFIG_NOISE_AMPLITUDE) && defined(CONFIG_PITCHNOISE_ENABLE)
        globalAircraft.autopilot.reportPitch(sim_flight_data_args.pitch->dval[0] + getWhiteNoiseNumber(CONFIG_NOISE_AMPLITUDE));
    #else
        globalAircraft.autopilot.reportPitch(sim_flight_data_args.pitch->dval[0]);
    #endif

    #if defined(CONFIG_NOISE_ENABLE) && defined(CONFIG_NOISE_AMPLITUDE) && defined(CONFIG_ROLLNOISE_ENABLE)
        globalAircraft.autopilot.reportPitch(sim_flight_data_args.roll->dval[0] + getWhiteNoiseNumber(CONFIG_NOISE_AMPLITUDE)));
    #else
        globalAircraft.autopilot.reportRoll(sim_flight_data_args.roll->dval[0]);
    #endif

    #if defined(CONFIG_NOISE_ENABLE) && defined(CONFIG_NOISE_AMPLITUDE) && defined(CONFIG_HEIGHTNOISE_ENABLE)
        globalAircraft.autopilot.reportCoordinate({sim_flight_data_args.latitude->dval[0],sim_flight_data_args.longitude->dval[0],sim_flight_data_args.altitude->dval[0] + getWhiteNoiseNumber(CONFIG_NOISE_AMPLITUDE)});
    #else
        globalAircraft.autopilot.reportCoordinate({sim_flight_data_args.latitude->dval[0],sim_flight_data_args.longitude->dval[0],sim_flight_data_args.altitude->dval[0]});
    #endif
 
    //globalAircraft.autopilot.reportYaw(sim_flight_data_args.yaw->dval[0]);
    
    globalAircraft.autopilot.reportCompass(sim_flight_data_args.compass->dval[0]);
    globalAircraft.printActuators();
    return 0;
}

void register_sim(void)
{
    randomInit();
    sim_flight_data_args.pitch = arg_dbl1(NULL, "pit", "<double>", "Pitch");
    sim_flight_data_args.roll = arg_dbl1(NULL, "rol", "<double>", "Roll");
    sim_flight_data_args.yaw = arg_dbl1(NULL, "yaw", "<double>", "Yaw");
    sim_flight_data_args.altitude = arg_dbl1(NULL, "alt", "<double>", "Altitude");
    sim_flight_data_args.longitude = arg_dbl1(NULL, "lon", "<double>", "Longitude");
    sim_flight_data_args.latitude = arg_dbl1(NULL, "lat", "<double>", "Latitude");
    sim_flight_data_args.compass = arg_dbl1(NULL, "com", "<double>", "Compass");
    sim_flight_data_args.end = arg_end(7);

    const esp_console_cmd_t sim_flight_data_cmd = {
        .command = "sim_flight_data",
        .help = "Get simulation data from flightgear",
        .hint = NULL,
        .func = &funcSimFlightData,
        .argtable = &sim_flight_data_args};

    ESP_ERROR_CHECK(esp_console_cmd_register(&sim_flight_data_cmd));
}