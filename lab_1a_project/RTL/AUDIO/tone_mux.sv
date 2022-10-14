
module	tone_mux	(	
					input		logic	clk,
					input		logic	resetN,
					
					input	logic	[3:0] tone_in_1,
					input    logic en1,
					input	logic	[3:0] tone_in_2,
					input    logic en2,
					input	logic	[3:0] tone_in_3,
					input    logic en3,
					input	logic	[3:0] tone_in_4,
					input    logic en4,
					input	logic	[3:0] tone_in_5,
					input    logic en5,
					
					output logic enable,
					output	logic	[3:0] tone_out

);

assign enable = en1 || en2 || en3 || en4 || en5;

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
		tone_out <= 0;
	end
	else begin
		if( en1 ) begin
			tone_out <= tone_in_1;
		end
		else if( en2 ) begin
			tone_out <= tone_in_2;
		end
		else if( en3 ) begin
			tone_out <= tone_in_3;
		end
		else if( en4 ) begin
			tone_out <= tone_in_4;
		end
		else if( en5 ) begin
			tone_out <= tone_in_5;
		end
		else begin
			tone_out <= 0;
		end
		
	end
end



endmodule
