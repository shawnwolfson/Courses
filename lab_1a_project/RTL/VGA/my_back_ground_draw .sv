//-- feb 2021 add all colors square 
// (c) Technion IIT, Department of Electrical Engineering 2021


module	my_back_ground_draw	(	

					input	logic	clk,
					input	logic	resetN,
					input 	logic	[10:0]	pixelX,
					input 	logic	[10:0]	pixelY,

					output	logic	[7:0]	BG_RGB,
					output	logic		boardersDrawReq 
);

const int	xFrameSize	=	635;
const int	yFrameSize	=	475;
const int	bracketOffset =	30;
const int   COLOR_MARTIX_SIZE  = 16*8 ; // 128 

logic [2:0] redBits;
logic [2:0] greenBits;
logic [1:0] blueBits;
logic [10:0] shift_pixelX;


localparam logic [2:0] DARK_COLOR = 3'b111 ;// bitmap of a dark color
localparam logic [2:0] LIGHT_COLOR = 3'b000 ;// bitmap of a light color

 
localparam  int FIRST_Y_BORDER  = 0 ;
localparam  int SECOND_Y_BORDER  = 0 ;
localparam  int THIRD_Y_BORDER  = 100 ;
localparam  int FOURTH_Y_BORDER  = 100 ;

localparam  int FIRST_X_BORDER  = 430 ;
localparam  int SECOND_X_BORDER  = 440 ;
localparam  int THIRD_X_BORDER  = 450 ;
localparam  int FOURTH_X_BORDER  = 460 ;
localparam  int FIFTH_X_BORDER  = 470 ;
localparam  int SIXTH_X_BORDER  = 500 ;
localparam  int SEVENTH_X_BORDER  = 510 ;
localparam  int EIGHTH_X_BORDER  = 530 ;

parameter  logic [10:0] COLOR_MATRIX_TOP_Y  = 100 ; 
parameter  logic [10:0] COLOR_MATRIX_LEFT_X = 100 ;

 

// this is a block to generate the background 
  
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
				redBits <= DARK_COLOR ;	
				greenBits <= DARK_COLOR  ;	
				blueBits <= DARK_COLOR ;	 
	end 
	else begin

	// defaults for borders
		redBits <= 3'b011 ;
		greenBits <= 3'b001; 
		blueBits <= 3'b000;
		boardersDrawReq <= 	1'b1 ; 
 
	//for the field
		if ((pixelX > bracketOffset && pixelX < (xFrameSize-bracketOffset)) && //cyan
			(pixelY > bracketOffset && pixelY < (yFrameSize-bracketOffset)))
			begin 
					redBits <= 3'b000 ;	
					greenBits <= 3'b111  ;	
					blueBits <= 3'b011 ;
					boardersDrawReq <= 	1'b0 ; // pulse if drawing the boarders 
			end	
		if ((pixelX > bracketOffset && pixelX < (xFrameSize-bracketOffset)) && // gray
			(pixelY > (yFrameSize- 12 *bracketOffset) && pixelY < (yFrameSize-bracketOffset)))
			begin
			if ((pixelX > FOURTH_X_BORDER && pixelX < SIXTH_X_BORDER) && (pixelY >= (yFrameSize- 12 * bracketOffset) && pixelY <= (yFrameSize- 12 * bracketOffset + 10) )) // top up line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > FOURTH_X_BORDER && pixelX < FIFTH_X_BORDER) && (pixelY >= (yFrameSize- 12 * bracketOffset) && pixelY <= (yFrameSize- 10 * bracketOffset - 10) )) // left up line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > 490 && pixelX < SIXTH_X_BORDER) && (pixelY >= (yFrameSize- 12 * bracketOffset) && pixelY <= (yFrameSize- 10 * bracketOffset - 10) )) // right up line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > FOURTH_X_BORDER && pixelX < SIXTH_X_BORDER) && (pixelY >= (yFrameSize- 10 * bracketOffset - 20) && pixelY <= (yFrameSize- 10 *bracketOffset - 10) )) // bot up line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end	
			else if ((pixelX > THIRD_X_BORDER && pixelX < SEVENTH_X_BORDER) && (pixelY >= (yFrameSize- 10 * bracketOffset - 10) && pixelY <= (yFrameSize- 10 * bracketOffset) )) // top line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > THIRD_X_BORDER && pixelX < FOURTH_X_BORDER) && (pixelY >= (yFrameSize- 10 * bracketOffset - 10) && pixelY <= (yFrameSize- 8 * bracketOffset) )) // left line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > SIXTH_X_BORDER && pixelX < SEVENTH_X_BORDER) && (pixelY >= (yFrameSize- 10 * bracketOffset - 10) && pixelY <= (yFrameSize- 8 * bracketOffset) )) // right line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > THIRD_X_BORDER && pixelX < SEVENTH_X_BORDER) && (pixelY >= (yFrameSize- 8 * bracketOffset - 10) && pixelY <= (yFrameSize- 8 *bracketOffset) )) // bot line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else begin
					/*redBits <= 3'b011 ;	
					greenBits <= 3'b011 ;	
					blueBits <= 3'b001 ;*/
					redBits <= 3'b000 ;	
					greenBits <= 3'b111  ;	
					blueBits <= 3'b011 ;
					boardersDrawReq <= 	1'b0 ; // pulse if drawing the boarders
					end
			end

		if ((pixelX > bracketOffset && pixelX < (xFrameSize-bracketOffset)) && // green
			(pixelY > (yFrameSize- 8 *bracketOffset) && pixelY < (yFrameSize-bracketOffset)))
			begin
			if ((pixelX > FIRST_X_BORDER && pixelX < EIGHTH_X_BORDER) && (pixelY >= (yFrameSize- 8 * bracketOffset) && pixelY <= (yFrameSize- 8 * bracketOffset + 10) )) // top line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > FIRST_X_BORDER && pixelX < SECOND_X_BORDER) && (pixelY >= (yFrameSize- 8 * bracketOffset) && pixelY <= (yFrameSize- 4 * bracketOffset) )) // left line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > 520 && pixelX < EIGHTH_X_BORDER) && (pixelY >= (yFrameSize- 8 * bracketOffset) && pixelY <= (yFrameSize- 4 * bracketOffset) )) // right line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else if ((pixelX > FIRST_X_BORDER && pixelX < EIGHTH_X_BORDER) && (pixelY >= (yFrameSize- 4 * bracketOffset - 10) && pixelY <= (yFrameSize- 4 *bracketOffset) )) // bot line
				begin 
						redBits <= 3'b011 ;
						greenBits <= 3'b001; 
						blueBits <= 3'b000;
				end
			else begin
					if((pixelX > bracketOffset && pixelX < (xFrameSize-bracketOffset)) && (pixelY > (yFrameSize- 8 *bracketOffset) && pixelY < (yFrameSize - 6 * bracketOffset))) begin
						redBits <= 3'b011 ;	
						greenBits <= 3'b011 ;	
						blueBits <= 3'b001 ;
					end
					else begin
						redBits <= 3'b000 ;	
						greenBits <= 3'b011 ;	
						blueBits <= 3'b000 ;
						boardersDrawReq <= 	1'b0 ; // pulse if drawing the boarders 					
					end
				end
			end
		if ((pixelX > bracketOffset && pixelX < (xFrameSize-bracketOffset)) && // blue
			(pixelY > (yFrameSize- 4 *bracketOffset) && pixelY < (yFrameSize-bracketOffset)))
			begin 
					redBits <= 3'b000 ;	
					greenBits <= 3'b000 ;	
					blueBits <= 3'b001 ;
					boardersDrawReq <= 	1'b0 ; // pulse if drawing the boarders 
			end		
	BG_RGB =  {redBits , greenBits , blueBits} ; //collect color nibbles to an 8 bit word 
			


	end; 	
end 
endmodule

