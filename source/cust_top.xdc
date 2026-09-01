# constraints for custom board

set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 2.5 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 6 [current_design]
set_property BITSTREAM.Config.SPI_BUSWIDTH 4 [current_design]
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

create_clock -period 10.000 -name clk100 -waveform {0.000 5.000} [get_ports {clk100}]

# Master clock
set_property IOSTANDARD LVCMOS33    [get_ports {clk100}];
set_property PACKAGE_PIN B8         [get_ports {clk100}]

# USB Uart
set_property IOSTANDARD LVCMOS33    [get_ports {usb_uart_*}]
set_property PACKAGE_PIN U17        [get_ports {usb_uart_rxd}]
set_property PACKAGE_PIN M13        [get_ports {usb_uart_txd}]

# TEC SPI
set_property IOSTANDARD LVCMOS33    [get_ports {max_spi_*}]
set_property PACKAGE_PIN H17        [get_ports {max_spi_mosi}]
set_property PACKAGE_PIN  B9        [get_ports {max_spi_miso}]
set_property PACKAGE_PIN A14        [get_ports {max_spi_sck}]
set_property PACKAGE_PIN A11        [get_ports {max_spi_ss[0]}]
set_property PACKAGE_PIN  C9        [get_ports {max_spi_ss[1]}]
set_property IOSTANDARD LVCMOS25    [get_ports {max_cnvtb}]
set_property PACKAGE_PIN  K2        [get_ports {max_cnvtb}]

set_property DRIVE 4                [get_ports {tec_ref_clk}]
set_property SLEW SLOW              [get_ports {tec_ref_clk}]
set_property IOSTANDARD LVCMOS33    [get_ports {tec_ref_clk}]
set_property PACKAGE_PIN A13        [get_ports {tec_ref_clk}]

# Discrete LEDs
set_property IOSTANDARD LVCMOS33    [get_ports {led[*]}]
set_property PACKAGE_PIN V11        [get_ports {led[3]}]
set_property PACKAGE_PIN V10        [get_ports {led[2]}]
set_property PACKAGE_PIN T11        [get_ports {led[1]}]
set_property PACKAGE_PIN  T9        [get_ports {led[0]}]
