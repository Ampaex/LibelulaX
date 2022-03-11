idf_component_register(SRCS "autopilot.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES pidController aircraft gps)
