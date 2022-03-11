idf_component_register(SRCS "cmd_system.c" "cmd_nvs.c" "cmd_wifi.c" "cmd_pidtest.cpp" "cmd_sim.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES console spi_flash nvs_flash aircraft)
