//
module top (
    input   logic           clkin100,
    output  logic[7:0]      led
);

    logic clk;
    assign clk = clkin100;

    logic[31:0] led_count = 0;
    always_ff @(posedge clk) begin
        led_count <= led_count + 1;
        led <= led_count[31-:8];
    end
    
    // debug
    led_ila ila_inst (.clk(clk), .probe0(led_count)); // 32

endmodule
