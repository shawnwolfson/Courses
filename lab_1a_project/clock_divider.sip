set_global_assignment -entity "clock_divider" -library "lib_clock_divider" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "clock_divider" -library "lib_clock_divider" -name IP_TOOL_VERSION "17.0"
set_global_assignment -entity "clock_divider" -library "lib_clock_divider" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_clock_divider" -name SPD_FILE [file join $::quartus(sip_path) "clock_divider.spd"]

set_global_assignment -library "lib_clock_divider" -name MISC_FILE [file join $::quartus(sip_path) "clock_divider_sim/clock_divider.vo"]
