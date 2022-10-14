//
// coding convention dudy December 2018
// (c) Technion IIT, Department of Electrical Engineering 2021
// generating a number bitmap 



module MinusBitMap	(	
					input		logic	clk,
					input		logic	resetN,
					input 	logic	[10:0] offsetX,// offset from top left  position 
					input 	logic	[10:0] offsetY,
					input		logic	InsideRectangle, //input that the pixel is within a bracket 
					input 	logic on_or_off_input, // digit to display
					
					output	logic				drawingRequest, //output that the pixel should be dispalyed 
					output	logic	[7:0]		RGBout
);
// generating a smily bitmap 

parameter  logic	[7:0] digit_color = 8'hff ; //set the color of the digit 


bit [0:31] [0:15] number_bitmap  = 


{16'b 0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	1111111111111111,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000,
16'b	0000000000000000

}; 
																	



// pipeline (ff) to get the pixel color from the array 	 

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
		drawingRequest <=	1'b0;
	end
	else begin
		drawingRequest <=	1'b0;
	
		if (InsideRectangle == 1'b1) begin
			if(on_or_off_input == 1'b1) begin
				drawingRequest <= (number_bitmap[offsetY][offsetX]);	//get value from bitmap  
			end
		end
	end 
end

assign RGBout = digit_color ; // this is a fixed color 

endmodule