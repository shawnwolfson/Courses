// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- Alex Grinshpun Apr 2017
//-- Dudy Nov 13 2017
// SystemVerilog version Alex Grinshpun May 2018
// coding convention dudy December 2018
// updaed Eyal Lev Feb 2021


module	Check_Ball_Moving(	
 
					input	logic	clk,
					input	logic	resetN,
					input	logic	startOfFrame,  // short pulse every start of frame 30Hz 

					input logic white_moving,
					input logic red_moving,
					input logic black_moving,
					
					output logic allow_cursor_hit
					
);

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin 
		allow_cursor_hit <= 1'b0;
	end 
	else begin
		allow_cursor_hit <= !(white_moving || red_moving || black_moving); //allow can cont only when all balls stopped
	end
end

//assign moving = !white_moving;

endmodule