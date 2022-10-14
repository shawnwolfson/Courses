// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- Alex Grinshpun Apr 2017
//-- Dudy Nov 13 2017
// SystemVerilog version Alex Grinshpun May 2018
// coding convention dudy December 2018
// updaed Eyal Lev Feb 2021

//Module to Control the cursor
module	Sound_out	(	
 
					input	logic	clk,
					input	logic	resetN,
					input	logic	startOfFrame,  // short pulse every start of frame 30Hz 

					input logic make_sound,


					output logic enable,
					output	logic [3:0] tone
);


parameter int sound_sel = 0;
logic [8:0] [3:0] sound = { 
	4'd1,
	4'd2,
	4'd3,
	4'd4,
	4'd5,
	4'd6,
	4'd7,
	4'd8,
	4'd9,
};

int iterator = 0;
int finished = 1;

always_ff@(posedge clk or negedge resetN) begin
	if(!resetN) begin
		iterator <= 0;
		enable <= 0;
		finished <= 1;
	end
	else begin
		if(make_sound) begin 
			finished <= 0;
		end
		/**else begin
			finished <= finished;
		end**/
		if(!finished) begin 
			if(iterator < 15) begin
				if(startOfFrame) begin 
					iterator <= iterator + 1;
				end
				enable <=1;
				tone <= sound[sound_sel];
			end
		end
		else begin
			finished <= 1;
			enable <= 0;
			iterator <= 0;
		end
	end
end


endmodule
