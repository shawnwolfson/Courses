//-- feb 2021 add all colors square 
// (c) Technion IIT, Department of Electrical Engineering 2021


module	back_ground_drawSquare	(	

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

localparam logic [2:0] DARK_COLOR = 3'b111 ;// bitmap of a dark color
localparam logic [2:0] LIGHT_COLOR = 3'b000 ;// bitmap of a light color

 
localparam  int RED_TOP_Y  = 156 ;
localparam  int RED_LEFT_X  = 256 ;
localparam  int GREEN_RIGHT_X  = 32 ;
localparam  int BLUE_BOTTOM_Y  = 300 ;
localparam  int BLUE_RIGHT_X  = 200 ;
 
parameter  int COLOR_MATRIX_TOP_Y  = 100 ; 
parameter  int COLOR_MATRIX_LEFT_X = 100 ;

 

// this is a block to generate the background 
//it has four sub modules : 

	// 1. draw the yellow borders
	// 2. draw four lines with "bracketOffset" offset from the border 
	// 3.  draw red rectangle at the bottom right,  green on the left, and blue on top left 
	// 4. draw a matrix of 16*16 rectangles with all the colors, each rectsangle 8*8 pixels  	

 
 
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
				redBits <= DARK_COLOR ;	
				greenBits <= DARK_COLOR  ;	
				blueBits <= DARK_COLOR ;	 
	end 
	else begin

	// defaults 
		greenBits <= 3'b110 ; 
		redBits <= 3'b010 ;
		blueBits <= LIGHT_COLOR;
		boardersDrawReq <= 	1'b0 ; 

					
	// draw the yellow borders 
		if (pixelX == 0 || pixelY == 0  || pixelX == xFrameSize || pixelY == yFrameSize)
			begin 
				redBits <= DARK_COLOR ;	
				greenBits <= DARK_COLOR ;	
				blueBits <= LIGHT_COLOR ;	// 3rd bit will be truncked
			end
		// draw  four lines with "bracketOffset" offset from the border 
		
		if (        pixelX == bracketOffset ||
						pixelY == bracketOffset ||
						pixelX == (xFrameSize-bracketOffset) || 
						pixelY == (yFrameSize-bracketOffset)) 
			begin 
					redBits <= DARK_COLOR ;	
					greenBits <= DARK_COLOR  ;	
					blueBits <= DARK_COLOR ;
					boardersDrawReq <= 	1'b1 ; // pulse if drawing the boarders 
			end
	
	// note numbers can be used inline if they appear only once 


	
	// 3.  draw red rectangle at the bottom right,  green on the left, and blue on top left 
	//-------------------------------------------------------------------------------------
		
		if (pixelX > RED_TOP_Y && pixelY >= RED_LEFT_X ) // rectangles on part of the screen 
				redBits <= DARK_COLOR ; 
				 
	
		if (GREEN_RIGHT_X <  32  ) 
				greenBits <= 3'b011 ; 
						
		if (pixelX <  BLUE_RIGHT_X && pixelY < BLUE_BOTTOM_Y )   
					blueBits <= 2'b10  ; 

				

	// 4. draw a matrix of 16*16 rectangles with all the colors, each rectsangle 8*8 pixels  	
   // ---------------------------------------------------------------------------------------

// 		if ((pixelX > 32)  && (pixelX < 160) && ( pixelY > 32) && (pixelY < 160))  begin
		if ((pixelX > COLOR_MATRIX_LEFT_X)  && (pixelX < COLOR_MATRIX_LEFT_X + COLOR_MARTIX_SIZE) 
		&& ( pixelY > COLOR_MATRIX_TOP_Y) && (pixelY < COLOR_MATRIX_TOP_Y + COLOR_MARTIX_SIZE )) 
		begin
			redBits <= pixelX[5:3] ; 
			greenBits <= pixelY[5:3] ; 
			blueBits <= { pixelX[6] , pixelY[6]} ; 


	          
		end	

		
	BG_RGB =  {redBits , greenBits , blueBits} ; //collect color nibbles to an 8 bit word 
			


	end; 	
end 
endmodule

