// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- Alex Grinshpun Apr 2017
//-- Dudy Nov 13 2017
// SystemVerilog version Alex Grinshpun May 2018
// coding convention dudy December 2018
// updaed Eyal Lev Feb 2021

//Module to control the BLACK ball movement. The BLACK ball is assosiated with the BLACK bird
module	BLACK_Ball_moveCollision	(	
 
					input	logic	clk,
					input	logic	resetN,
					input	logic	startOfFrame,  // short pulse every start of frame 30Hz 
				
				
					input logic signed [10:0] white_ball_X_speed,
					input logic signed [10:0] white_ball_Y_speed,
					input logic signed [10:0] red_ball_X_speed,
					input logic signed [10:0] red_ball_Y_speed,
					
					
					input logic collision_black_ball_brackets,
					input logic collision_black_ball_white_ball,
					input logic collision_black_ball_red_ball,
					input logic collision_black_ball_hole,
					
					input logic [10:0] white_ball_X,
					input logic [10:0] white_ball_Y,
					input logic [10:0] red_ball_X,
					input logic [10:0] red_ball_Y,
					
					input logic signed [10:0] black_ball_X_speed_new_from_white,
					input logic signed [10:0] black_ball_Y_speed_new_from_white,
					input logic signed [10:0] black_ball_X_speed_new_from_red,
					input logic signed [10:0] black_ball_Y_speed_new_from_red,
					
					input logic collision_black_ball_fast_speed_gift,
					input logic collision_black_ball_slow_speed_gift,
					input	logic	[3:0] HitEdgeCode, //one bit per edge 
					
					
					output	 logic signed 	[10:0]	topLeftX, // output the top left corner 
					output	 logic signed	[10:0]	topLeftY,  // can be negative , if the object is partliy outside 
					output logic moving,
					output logic signed [10:0] black_ball_Y_speed, 
					output logic signed [10:0] black_ball_X_speed,
					output logic signed [10:0] white_ball_X_new_speed_from_black,
					output logic signed [10:0] white_ball_Y_new_speed_from_black,
					output logic signed [10:0] red_ball_X_new_speed_from_black,
					output logic signed [10:0] red_ball_Y_new_speed_from_black
);


// a module used to generate the  ball trajectory.  

parameter int INITIAL_X = 150;
parameter int INITIAL_Y = 185;
parameter int INITIAL_X_SPEED = 0;
parameter int INITIAL_Y_SPEED = 0;

int Yspeed_absolute = 0;
int Xspeed_absolute = 0;
int Y_ACCEL = INITIAL_ACCEL;
int X_ACCEL = INITIAL_ACCEL;

int collision_freeze_y_with_white_ball = 0;
int collision_freeze_y_with_red_ball = 0;
int collision_freeze_x_with_white_ball = 0;
int collision_freeze_x_with_red_ball = 0;

const int	FIXED_POINT_MULTIPLIER	=	64;
// FIXED_POINT_MULTIPLIER is used to enable working with integers in high resolution so that 
// we do all calculations with topLeftX_FixedPoint to get a resolution of 1/64 pixel in calcuatuions,
// we devide at the end by FIXED_POINT_MULTIPLIER which must be 2^n, to return to the initial proportions
const int	x_FRAME_SIZE	=	639 * FIXED_POINT_MULTIPLIER; // note it must be 2^n 
const int	y_FRAME_SIZE	=	479 * FIXED_POINT_MULTIPLIER;

const int	bracketOffset =	30 * FIXED_POINT_MULTIPLIER;

const int   OBJECT_WIDTH_X = 32 * FIXED_POINT_MULTIPLIER;
const int 	OBJECT_WIDTH_Y = 32 * FIXED_POINT_MULTIPLIER;
const int 	OBJECT_SIZE = 31 * FIXED_POINT_MULTIPLIER;
const int 	HALF_OBJECT_SIZE = 10 * FIXED_POINT_MULTIPLIER;

const int NO_SPEED = 4;
localparam int INITIAL_ACCEL = 3;
localparam int MIN_SPEED = 5;
localparam int MIN_DIAGONAL_SPEED = 6;
const int dead = 3;

int Xspeed, topLeftX_FixedPoint; // local parameters 
int Yspeed, topLeftY_FixedPoint;

int temp_black_ball_Y_speed;
int temp_black_ball_X_speed;

int corrected_white_ball_X = 0; 
int corrected_white_ball_Y = 0; 
int corrected_red_ball_X = 0; 
int corrected_red_ball_Y = 0; 

int Y_distance_between_black_white = 0;
int X_distance_between_black_white = 0;
int Y_distance_between_black_red = 0;
int X_distance_between_black_red = 0;

int white_ball_hit_from_left = 0;
int white_ball_hit_from_top = 0;
int white_ball_hit_from_right = 0;
int white_ball_hit_from_bottom = 0;

int red_ball_hit_from_left = 0;
int red_ball_hit_from_top = 0;
int red_ball_hit_from_right = 0;
int red_ball_hit_from_bottom = 0;

int brackets_freeze_y = 0;
int brackets_freeze_x = 0;
int dead_frames_brackets_y = 0;
int dead_frames_brackets_x = 0;


int Xspeed_absolute_white = 0;
int Yspeed_absolute_white = 0;

int Xspeed_absolute_red = 0;
int Yspeed_absolute_red = 0;

//Block to calculate the absolute values and other important paramters like distances
always_comb begin
	Xspeed_absolute = (Xspeed < 0) ? -Xspeed : Xspeed;
	Yspeed_absolute = (Yspeed < 0) ? -Yspeed : Yspeed;
	
	Xspeed_absolute_white = (white_ball_X_speed < 0) ? -white_ball_X_speed : white_ball_X_speed;
	Yspeed_absolute_white = (white_ball_Y_speed < 0) ? -white_ball_Y_speed : white_ball_Y_speed;

	Xspeed_absolute_red = (red_ball_X_speed < 0) ? -red_ball_X_speed : red_ball_X_speed;
	Yspeed_absolute_red = (red_ball_Y_speed < 0) ? -red_ball_Y_speed : red_ball_Y_speed;
	
	corrected_white_ball_X = white_ball_X * FIXED_POINT_MULTIPLIER; 
	corrected_white_ball_Y = white_ball_Y * FIXED_POINT_MULTIPLIER;

	corrected_red_ball_X = red_ball_X * FIXED_POINT_MULTIPLIER; 
	corrected_red_ball_Y = red_ball_Y * FIXED_POINT_MULTIPLIER;
	
	X_distance_between_black_white = ((corrected_white_ball_X - topLeftX_FixedPoint) > 0) ? (corrected_white_ball_X - topLeftX_FixedPoint) : (topLeftX_FixedPoint - corrected_white_ball_X);
	Y_distance_between_black_white = ((corrected_white_ball_Y - topLeftY_FixedPoint) > 0) ? (corrected_white_ball_Y - topLeftY_FixedPoint) : (topLeftY_FixedPoint - corrected_white_ball_Y);

	X_distance_between_black_red = ((corrected_red_ball_X - topLeftX_FixedPoint) > 0) ? (corrected_red_ball_X - topLeftX_FixedPoint) : (topLeftX_FixedPoint - corrected_red_ball_X);
	Y_distance_between_black_red = ((corrected_red_ball_Y - topLeftY_FixedPoint) > 0) ? (corrected_red_ball_Y - topLeftY_FixedPoint) : (topLeftY_FixedPoint - corrected_red_ball_Y);
	
	white_ball_hit_from_left = ((corrected_white_ball_X < (topLeftX_FixedPoint)));
	white_ball_hit_from_top = ((corrected_white_ball_Y < (topLeftY_FixedPoint)));
	white_ball_hit_from_right = ((corrected_white_ball_X > (topLeftX_FixedPoint)));
	white_ball_hit_from_bottom = ((corrected_white_ball_Y > (topLeftY_FixedPoint)));
	
	red_ball_hit_from_left = ((corrected_red_ball_X < (topLeftX_FixedPoint)));
	red_ball_hit_from_top = ((corrected_red_ball_Y < (topLeftY_FixedPoint)));
	red_ball_hit_from_right = ((corrected_red_ball_X > (topLeftX_FixedPoint)));
	red_ball_hit_from_bottom = ((corrected_red_ball_Y > (topLeftY_FixedPoint)));
end

//////////--------------------------------------------------------------------------------------------------------------=
//  calculation 0f Y Axis speed


always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin 
		Yspeed	<= INITIAL_Y_SPEED;
		topLeftY_FixedPoint	<= INITIAL_Y * FIXED_POINT_MULTIPLIER;
	end 
	else begin
		if(brackets_freeze_y == 0 && ((topLeftY_FixedPoint <= bracketOffset) || ((topLeftY_FixedPoint + OBJECT_WIDTH_Y)  >= (y_FRAME_SIZE - bracketOffset)))) begin
				  Yspeed <= -Yspeed ; 
				  brackets_freeze_y <= 1;
		end
		
		if(collision_black_ball_hole) begin
			Yspeed <= 0;
		end			
		
		//calculation of speeds during collision with white ball
		if( collision_freeze_y_with_white_ball == 0 && collision_black_ball_white_ball) begin //white ball hit black ball
			if(Xspeed_absolute_white == 0 && Yspeed_absolute_white == 0) begin // if white ball is stationary, treat black ball as white and white ball as black
				Yspeed <= black_ball_Y_speed_new_from_white;
			end
			else begin // white ball is not stationary
				white_ball_Y_new_speed_from_black <= 0;
				if((white_ball_hit_from_left || white_ball_hit_from_right) && (white_ball_hit_from_top || white_ball_hit_from_bottom)) begin // one of the edges
					if(Yspeed_absolute_white < MIN_DIAGONAL_SPEED) begin //basically no speed in Y axis, all speed comes from X axis
						if(white_ball_hit_from_top) begin //one of the top edges. Black ball Y speed is positive, White ball Y speed is negative.
							Yspeed <=  Xspeed_absolute_white / 2;
							white_ball_Y_new_speed_from_black <=  - Xspeed_absolute_white / 2;
						end
						else if(white_ball_hit_from_bottom) begin //one of the bottom edges. Black ball Y speed is negative, White ball Y speed is positive.
							Yspeed <=  -Xspeed_absolute_white / 2;
							white_ball_Y_new_speed_from_black <=  Xspeed_absolute_white / 2;
						end
					end
					else begin // There is some Y axis speed
						Yspeed <= white_ball_Y_speed / 2;
						white_ball_Y_new_speed_from_black <= white_ball_Y_speed / 2;
					end
				end
				else begin //not an edge
					Yspeed <= white_ball_Y_speed / 2;
					white_ball_Y_new_speed_from_black <= white_ball_Y_speed / 2;
				end
			end
			collision_freeze_y_with_white_ball <= 1;
		end
		
		if(collision_freeze_y_with_white_ball && !collision_black_ball_white_ball && (Y_distance_between_black_white > OBJECT_SIZE)) begin
				collision_freeze_y_with_white_ball <= 0;
		end

		//calculation of speeds during collision with red ball
		if( collision_freeze_y_with_red_ball == 0 && collision_black_ball_red_ball) begin //red ball hit black ball
			if(Xspeed_absolute_red == 0 && Yspeed_absolute_red == 0) begin // if red ball is stationary, treat black ball as red and red ball as black
				Yspeed <= black_ball_Y_speed_new_from_red;
			end
			else begin // white ball is not stationary
				red_ball_Y_new_speed_from_black <= 0;
				if((red_ball_hit_from_left || red_ball_hit_from_right) && (red_ball_hit_from_top || red_ball_hit_from_bottom)) begin // one of the edges
					if(Yspeed_absolute_red < MIN_DIAGONAL_SPEED) begin //basically no speed in Y axis, all speed comes from X axis
						if(red_ball_hit_from_top) begin //one of the top edges. Black ball Y speed is positive, red ball Y speed is negative.
							Yspeed <=  Xspeed_absolute_red / 2;
							red_ball_Y_new_speed_from_black <=  - Xspeed_absolute_red / 2;
						end
						else if(red_ball_hit_from_bottom) begin //one of the bottom edges. Black ball Y speed is negative, red ball Y speed is positive.
							Yspeed <=  -Xspeed_absolute_red / 2;
							red_ball_Y_new_speed_from_black <=  Xspeed_absolute_red / 2;
						end
					end
					else begin // There is some Y axis speed
						Yspeed <= red_ball_Y_speed / 2;
						red_ball_Y_new_speed_from_black <= red_ball_Y_speed / 2;
					end
				end
				else begin //not an edge
					Yspeed <= red_ball_Y_speed / 2;
					red_ball_Y_new_speed_from_black <= red_ball_Y_speed / 2;
				end
			end
			collision_freeze_y_with_red_ball <= 1;
		end
		//unlock freeze
		if(collision_freeze_y_with_red_ball && !collision_black_ball_red_ball && (Y_distance_between_black_red > OBJECT_SIZE)) begin
				collision_freeze_y_with_red_ball <= 0;
		end
			
			
		if(collision_black_ball_fast_speed_gift) begin
			Yspeed <= 2 * Yspeed;
		end
		if(collision_black_ball_slow_speed_gift) begin
			Yspeed <= Yspeed / 2;
		end
		
		if (startOfFrame == 1'b1) begin 
				//Bracket freeze
			if(brackets_freeze_y) begin
				dead_frames_brackets_y <= dead_frames_brackets_y + 1 ;
			 end

			 if(dead_frames_brackets_y == dead) begin 
				dead_frames_brackets_y <= 0;
				brackets_freeze_y <= 0;
			 end	

				if (Yspeed < -NO_SPEED ) begin//  moving up
						Yspeed <= Yspeed  + Y_ACCEL ; 
				end
				else if (Yspeed > NO_SPEED ) begin //  moving down
						Yspeed <= Yspeed  - Y_ACCEL ; 	
				end
				else begin
					Yspeed <= INITIAL_Y_SPEED;
				end
				
				topLeftY_FixedPoint  <= topLeftY_FixedPoint + Yspeed; // position interpolation 
		end
		temp_black_ball_Y_speed <= Yspeed;
	end
end 

//////////--------------------------------------------------------------------------------------------------------------=
//  calculation of X Axis speed 

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin
		Xspeed	<= INITIAL_X_SPEED;
		topLeftX_FixedPoint	<= INITIAL_X * FIXED_POINT_MULTIPLIER;
	end
	else begin
		
		if(brackets_freeze_x == 0 && (topLeftX_FixedPoint <= bracketOffset || ((topLeftX_FixedPoint + OBJECT_WIDTH_X)  >= (x_FRAME_SIZE - bracketOffset)))) begin
					  Xspeed <= -Xspeed ; 
					  brackets_freeze_x <= 1;
		end
		
		if(collision_black_ball_hole) begin
					Xspeed <= 0;
		end

		//calculation of speeds during collision with white ball
		if( collision_freeze_x_with_white_ball == 0 && collision_black_ball_white_ball) begin //whiteball hit black ball 
			if(Xspeed_absolute_white == 0 && Yspeed_absolute_white == 0) begin // if white ball is stationary, treat black ball as white and white ball as black
				Xspeed <= black_ball_X_speed_new_from_white;
			end
			else begin
				white_ball_X_new_speed_from_black <= 0;
				if((white_ball_hit_from_left || white_ball_hit_from_right) && (white_ball_hit_from_top || white_ball_hit_from_bottom)) begin // one of the edges
					if(Xspeed_absolute_white < MIN_DIAGONAL_SPEED) begin //basically no speed in X axis, all speed comes from Y axis
						if(white_ball_hit_from_left) begin //one of the left edges. Black ball X speed is positive, white ball Y speed is negative.
							Xspeed <=  Yspeed_absolute_white / 2;
							white_ball_X_new_speed_from_black <=  - Yspeed_absolute_white / 2;
						end
						else if(white_ball_hit_from_right) begin //one of the right edges. Black ball X speed is negative, white ball X speed is positive.
							Xspeed <=  -Yspeed_absolute_white / 2;
							white_ball_X_new_speed_from_black <=  Yspeed_absolute_white / 2;
						end
					end
					else begin // There is some X axis speed
						Xspeed <= white_ball_X_speed / 2;
						white_ball_X_new_speed_from_black <= white_ball_X_speed / 2;
					end
				end
				else begin //not an edge
					Xspeed <= white_ball_X_speed / 2;
					white_ball_X_new_speed_from_black <= white_ball_X_speed / 2;
				end
			end
			collision_freeze_x_with_white_ball <= 1;
		end
		//unlock freeze
		if(collision_freeze_x_with_white_ball && !collision_black_ball_white_ball && (X_distance_between_black_white > OBJECT_SIZE)) begin
				collision_freeze_x_with_white_ball <= 0;
		end	
		
		//calculation of speeds during collision with red ball
		if( collision_freeze_x_with_red_ball == 0 && collision_black_ball_red_ball) begin //red ball hit black ball 
			if(Xspeed_absolute_red == 0 && Yspeed_absolute_red == 0) begin // if red ball is stationary, treat black ball as red and red ball as black
				Xspeed <= black_ball_X_speed_new_from_red;
			end
			else begin
				red_ball_X_new_speed_from_black <= 0;
				if((red_ball_hit_from_left || red_ball_hit_from_right) && (red_ball_hit_from_top || red_ball_hit_from_bottom)) begin // one of the edges
					if(Xspeed_absolute_red < MIN_DIAGONAL_SPEED) begin //basically no speed in X axis, all speed comes from Y axis
						if(red_ball_hit_from_left) begin //one of the left edges. Black ball X speed is positive, red ball Y speed is negative.
							Xspeed <=  Yspeed_absolute_red / 2;
							red_ball_X_new_speed_from_black <=  - Yspeed_absolute_red / 2;
						end
						else if(red_ball_hit_from_right) begin //one of the right edges. Black ball X speed is negative, red ball X speed is positive.
							Xspeed <=  -Yspeed_absolute_red / 2;
							red_ball_X_new_speed_from_black <=  Yspeed_absolute_red / 2;
						end
					end
					else begin // There is some X axis speed
						Xspeed <= red_ball_X_speed / 2;
						red_ball_X_new_speed_from_black <= red_ball_X_speed / 2;
					end
				end
				else begin //not an edge
					Xspeed <= red_ball_X_speed / 2;
					red_ball_X_new_speed_from_black <= red_ball_X_speed / 2;
				end
			end
			collision_freeze_x_with_red_ball <= 1;
		end
		//unlock freeze
		if(collision_freeze_x_with_red_ball && !collision_black_ball_red_ball && (X_distance_between_black_red > OBJECT_SIZE)) begin
				collision_freeze_x_with_red_ball <= 0;
		end

		
		if(collision_black_ball_fast_speed_gift) begin
			Xspeed <= 2 * Xspeed;
		end
		if(collision_black_ball_slow_speed_gift) begin
			Xspeed <= Xspeed / 2;
		end
		
		
		if (startOfFrame == 1'b1) begin 
		
		//Bracket freeze
			if(brackets_freeze_x) begin
				dead_frames_brackets_x <= dead_frames_brackets_x + 1 ;
			 end

			 if(dead_frames_brackets_x == dead) begin 
				dead_frames_brackets_x <= 0;
				brackets_freeze_x <= 0;
			 end	
		
			if (Xspeed < -NO_SPEED ) begin//  moving left
						Xspeed <= Xspeed  + X_ACCEL ; // deAccelerate : slow the speed down every clock tick 
			end
			
			else if (Xspeed > NO_SPEED ) begin//  moving right 
					Xspeed <= Xspeed  - X_ACCEL ; // deAccelerate : slow the speed down every clock tick 		
			end
			
			else begin
				Xspeed <= INITIAL_X_SPEED;
			end
					
					topLeftX_FixedPoint  <= topLeftX_FixedPoint + Xspeed; // position interpolation 	
			end
			temp_black_ball_X_speed <= Xspeed;
	end
end

//Control the friction so that the two axis will stop together
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin
		X_ACCEL	<= INITIAL_ACCEL;
		Y_ACCEL	<= INITIAL_ACCEL;
	end
	else begin
		X_ACCEL	<= INITIAL_ACCEL;
		Y_ACCEL	<= INITIAL_ACCEL;
		if(Xspeed_absolute > Yspeed_absolute) begin
			Y_ACCEL <= 0;
		end 
		else if(Xspeed_absolute < Yspeed_absolute) begin
			X_ACCEL <= 0;
		end 
		else begin
		X_ACCEL	<= INITIAL_ACCEL;
		Y_ACCEL	<= INITIAL_ACCEL;
		end
	end
end


//get a better (64 times) resolution using integer  

assign 	topLeftX = topLeftX_FixedPoint / FIXED_POINT_MULTIPLIER ;   // note it must be 2^n 
assign 	topLeftY = topLeftY_FixedPoint / FIXED_POINT_MULTIPLIER ;    
assign   moving = ((Xspeed == 0) && (Yspeed == 0)) ? 0 : 1;
assign black_ball_Y_speed = temp_black_ball_Y_speed;
assign black_ball_X_speed = temp_black_ball_X_speed;

endmodule
