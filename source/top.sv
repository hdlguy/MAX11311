//
module top (
    input   logic           clkin100,
    input   logic           rstn,
    output  logic[7:0]      led,
    input   logic           usb_uart_rxd,
    output  logic           usb_uart_txd
);

    logic clk;
    assign clk = clkin100;

    logic[31:0] led_count = 0;
    always_ff @(posedge clk) begin
        led_count <= led_count + 1;
        led <= led_count[31-:8];
    end
    
    system inst (
        //
        .clkin100(clk),
        .rstn(rstn),
        //    
        .uart_rtl_0_rxd(usb_uart_rx),
        .uart_rtl_0_txd(usb_uart_tx),
        //
        .spi_rtl_0_io0_i(spi_rtl_0_io0_i),
        .spi_rtl_0_io0_o(spi_rtl_0_io0_o),
        .spi_rtl_0_io0_t(spi_rtl_0_io0_t),
        .spi_rtl_0_io1_i(spi_rtl_0_io1_i),
        .spi_rtl_0_io1_o(spi_rtl_0_io1_o),
        .spi_rtl_0_io1_t(spi_rtl_0_io1_t),
        .spi_rtl_0_sck_i(spi_rtl_0_sck_i),
        .spi_rtl_0_sck_o(spi_rtl_0_sck_o),
        .spi_rtl_0_sck_t(spi_rtl_0_sck_t),
        .spi_rtl_0_ss_i(spi_rtl_0_ss_i),
        .spi_rtl_0_ss_o(spi_rtl_0_ss_o),
        .spi_rtl_0_ss_t(spi_rtl_0_ss_t)
    );
    
    // debug
    //led_ila ila_inst (.clk(clk), .probe0(led_count)); // 32

endmodule
