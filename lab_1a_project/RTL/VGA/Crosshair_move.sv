// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- Alex Grinshpun Apr 2017
//-- Dudy Nov 13 2017
// SystemVerilog version Alex Grinshpun May 2018
// coding convention dudy December 2018
// updaed Eyal Lev Feb 2021

//Module to Control the cursor
module	Crosshair_move	(	
 
					input	logic	clk,
					input	logic	resetN,
					input	logic	startOfFrame,  // short pulse every start of frame 30Hz 

					input logic Key_2_is_pressed,
					input logic Key_4_is_pressed,
					input logic Key_6_is_pressed,
					input logic Key_8_is_pressed,
					input logic Key_ENTER_is_pressed,
					
					
					input logic [10:0] white_ball_x,
					input logic [10:0] white_ball_y,
					input logic can_cont,

					output logic signed 	[10:0]	topLeftX, // output the top left corner 
					output logic signed	[10:0]	topLeftY,  // can be negative , if the object is partliy outside 
					output logic hit_accured,
					output logic shut_down
					
);


// FIXED_POINT_MULTIPLIER is used to enable working with integers in high resolution so that 
// we do all calculations with topLeftX_FixedPoint to get a resolution of 1/64 pixel in calcuatuions,
// we devide at the end by FIXED_POINT_MULTIPLIER which must be 2^n, to return to the initial proportions
localparam	x_FRAME_SIZE	=	639 * FIXED_POINT_MULTIPLIER; // note it must be 2^n 
localparam	y_FRAME_SIZE	=	479 * FIXED_POINT_MULTIPLIER;
localparam small_offset_correction = 3 * FIXED_POINT_MULTIPLIER;
const int	CURSOR_OFFSET =	15 * FIXED_POINT_MULTIPLIER;

parameter int INITIAL_CURSOR_X_POS = 50;
parameter int INITIAL_CURSOR_Y_POS = 185;

const int MAX_CURSOR_X_POS = x_FRAME_SIZE - CURSOR_OFFSET - OBJECT_SIZE - small_offset_correction;
const int MAX_CURSOR_Y_POS = y_FRAME_SIZE - CURSOR_OFFSET - OBJECT_SIZE - small_offset_correction;
const int MIN_CURSOR_X_POS = CURSOR_OFFSET;
const int MIN_CURSOR_Y_POS = CURSOR_OFFSET;

localparam int FIXED_POINT_MULTIPLIER	=	64;

localparam int HALF_OBJECT_SIZE = 16;
localparam int OBJECT_SIZE = 32 * FIXED_POINT_MULTIPLIER;


const int   OBJECT_WIDTH_X = 64;
const int addition = 2;
const int correct_addition = addition * FIXED_POINT_MULTIPLIER;

int white_ball_center_X; 
int white_ball_center_Y; 
int cursor_center_X;
int cursor_center_Y;
int Xspeed, topLeftX_FixedPoint = 0; // local parameters 
int Yspeed, topLeftY_FixedPoint = 0;
int temp_y_location = 200 * FIXED_POINT_MULTIPLIER;
int temp_x_location = 200 * FIXED_POINT_MULTIPLIER;
int temp_hit_accured;	
int enter_flag = 0; // to allow the cursor to return to white ball location only when enter was pressed

//////////////////////// another state machine for hit and enter key
//////////--------------------------------------------------------------------------------------------------------------=
//  calculation 0f Y Axis speed using gravity or colision

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin 
		white_ball_center_Y <= white_ball_y + HALF_OBJECT_SIZE;
		topLeftY_FixedPoint  <= INITIAL_CURSOR_Y_POS * FIXED_POINT_MULTIPLIER;
	end 
	else begin 
		//topLeftY_FixedPoint <= white_ball_y * FIXED_POINT_MULTIPLIER;
		//move the cursor
		if (startOfFrame == 1'b1) begin 
				white_ball_center_Y <= white_ball_y + HALF_OBJECT_SIZE; // position interpolation 
				cursor_center_Y = topLeftY_FixedPoint / FIXED_POINT_MULTIPLIER + HALF_OBJECT_SIZE;
					if ((Key_2_is_pressed) == 1'b1 && topLeftY_FixedPoint <= MAX_CURSOR_Y_POS)
						temp_y_location <= topLeftY_FixedPoint + correct_addition;
						
					else if ((Key_8_is_pressed == 1'b1 && topLeftY_FixedPoint >= MIN_CURSOR_Y_POS))
						temp_y_location <= topLeftY_FixedPoint - correct_addition;
						
					else if(can_cont && enter_flag) begin
						temp_y_location <= white_ball_center_Y * FIXED_POINT_MULTIPLIER;
						end
					
				topLeftY_FixedPoint <= temp_y_location;
				end
		end
end 

//  calculation 0f X Axis
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin 
			white_ball_center_X <= white_ball_x + HALF_OBJECT_SIZE;
			topLeftX_FixedPoint <= INITIAL_CURSOR_X_POS * FIXED_POINT_MULTIPLIER;
	end 
	else begin 
		//move the cursor 	
		if (startOfFrame == 1'b1) begin 
				white_ball_center_X <= white_ball_x + HALF_OBJECT_SIZE;
				cursor_center_X = topLeftX_FixedPoint / FIXED_POINT_MULTIPLIER + HALF_OBJECT_SIZE;
				// position interpolation 

				if ((Key_4_is_pressed == 1'b1 && topLeftX_FixedPoint >= MIN_CURSOR_X_POS)) 
					temp_x_location <= topLeftX_FixedPoint - correct_addition;
				
				else if ((Key_6_is_pressed == 1'b1 && topLeftX_FixedPoint <= MAX_CURSOR_X_POS))
					temp_x_location <= topLeftX_FixedPoint + correct_addition;
			
				else if(can_cont && enter_flag) begin
					temp_x_location <= white_ball_center_X * FIXED_POINT_MULTIPLIER;
					end
					
				topLeftX_FixedPoint <= temp_x_location;
			end
		end
end 

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin 
		temp_hit_accured <= 0;
		enter_flag <= 0;
	end 
	else begin 
		if (startOfFrame == 1'b1) begin 
				// position interpolation 
				if ((Key_ENTER_is_pressed == 1'b1 )) begin //while holding enter
						temp_hit_accured <= 1;
						enter_flag <= 1;
				end
				else if(can_cont && enter_flag) //reset enter flag
					enter_flag <= 0;
				else begin
					temp_hit_accured <= 0;
				end
			end
		end
end 
//get a better (64 times) resolution using integer

assign topLeftX = topLeftX_FixedPoint / FIXED_POINT_MULTIPLIER ;  
assign topLeftY = topLeftY_FixedPoint / FIXED_POINT_MULTIPLIER ;    
assign hit_accured = temp_hit_accured;
assign shut_down = (hit_accured || !can_cont) ? 1 : 0 ; // this signal is for killing the cursor while the red bird is moving
endmodule
