#include "cmd_pidtest.hpp"

#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include "pidController.hpp"

/** Arguments used by 'pid_test' function */
static struct
{
    struct arg_dbl *inputData;
    struct arg_dbl *setpoint;
    struct arg_end *end;
} pidtest_args;

static int pidtestFunc(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &pidtest_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, pidtest_args.end, argv[0]);
        return 1;
    }
    if(pidtest_args.setpoint->count > 0)
    {
        PIDdata setp = PIDdata(TEST, (float)pidtest_args.setpoint->dval[0]);
        setPIDsetpoint(setp);
    }
    
    PIDdata pidata = PIDdata(TEST, (float)pidtest_args.inputData->dval[0]);
    sendToPID(pidata);
    printf("%f\n", getPIDwithType(TEST)->getValue());
    return 0;
}

void register_pidtest(void)
{
    pidtest_args.inputData = arg_dbl1(NULL, NULL, "<double>", "Value to be evaluated");
    pidtest_args.setpoint = arg_dbl0("s", NULL, "<double>", "Setpoint for pid");
    pidtest_args.end = arg_end(2);

    const esp_console_cmd_t pidtest_cmd = {
        .command = "pidtest",
        .help = "Send data for testing the PID model",
        .hint = NULL,
        .func = &pidtestFunc,
        .argtable = &pidtest_args};

    ESP_ERROR_CHECK(esp_console_cmd_register(&pidtest_cmd));
}