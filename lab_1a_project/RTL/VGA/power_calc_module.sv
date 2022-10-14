




module	power_calc_module	(	
 
					input	logic	clk,
					input	logic	resetN,
					input	logic	startOfFrame, 
				
				
					input logic [3:0] keyPad,
				
					output logic [3:0] power
);
localparam INITIAL_POWER = 1;
localparam POWER_UNIT = 1;
int current_power = INITIAL_POWER;

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin
		current_power	<= INITIAL_POWER;
	end
	else begin
			if(keyPad == 3'h1) begin// plus button
				current_power <= 1;
			end
			else if(keyPad == 3'h3) begin
				current_power <= 2 ;
			end
			else if(keyPad == 3'h7) begin
				current_power <= 3 ;
			end
			else if(keyPad == 4'h9) begin
				current_power <= 4 ;
			end
	end
end
assign power = current_power;


endmodule