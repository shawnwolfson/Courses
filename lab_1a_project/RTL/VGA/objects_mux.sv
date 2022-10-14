
// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- Alex Grinshpun Apr 2017
//-- Dudy Nov 13 2017
// SystemVerilog version Alex Grinshpun May 2018
// coding convention dudy December 2018

//-- Eyal Lev 31 Jan 2021

module	objects_mux	(	
//		--------	Clock Input	 	
					input	logic	clk,
					input	logic	resetN,
					
			//Open screen
					input logic open_screen_draw,
					input	logic	[7:0] RGB_open_screen, 
					
			//End screens
					input logic end_screen_winner_draw,
					input	logic	[7:0] RGB_end_screen_winner, 
					
					input logic end_screen_loser_draw,
					input	logic	[7:0] RGB_end_screen_loser,					
			//labels
					input logic power_label_draw,
					input	logic	[7:0] RGB_power_label,
					
					input logic score_label_draw,
					input	logic	[7:0] RGB_score_label,
					
					input logic level_DR,
					input	logic	[7:0] levelRGB,
					
					input logic level_number_DR,
					input	logic	[7:0] level_numberRGB,
		   //cursor
					input logic cursor_DrawingRequest,
					input	logic	[7:0] cursorRGB, 
			//number and minus
					input logic number_DrawingRequest,
					input	logic	[7:0] numberRGB,	
					
					input logic minus_DrawingRequest,
					input	logic	[7:0] minusRGB,
				
			//power 
					input logic power_DrawingRequest,
					input	logic	[7:0] powerRGB,
					
			//balls:
					input logic white_ball_DrawingRequest,
					input	logic	[7:0] white_ballRGB, 
					
					input logic red_ball_DrawingRequest,
					input	logic	[7:0] red_ballRGB, 
					
					input logic black_ball_DrawingRequest,
					input	logic	[7:0] black_ballRGB, 	
			//targets
					input logic right_bot_target_DR,
					input	logic	[7:0] right_bot_targetRGB, 
					
					input logic left_top_target_DR,
					input	logic	[7:0] left_top_targetRGB, 
		
					input logic middle_bot_target_DR,
					input	logic	[7:0] middle_bot_targetRGB, 
					
					input logic right_top_target_DR,
					input	logic	[7:0] right_top_targetRGB, 

					input logic left_bot_target_DR,
					input	logic	[7:0] left_bot_targetRGB, 

					input logic middle_top_target_DR,
					input	logic	[7:0] middle_top_targetRGB, 
		
			//Holes:
			
			//added hole on middle top
					input logic left_top_hole_DrawingRequest,
					input logic [7:0] left_top_hole_RGB,
			//added hole on middle top
					input logic middle_top_hole_DrawingRequest,
					input logic [7:0] middle_top_hole_RGB,
			//added hole on middle top
					input logic right_top_hole_DrawingRequest,
					input logic [7:0] right_top_hole_RGB,
			//added hole on middle top
					input logic left_bottom_hole_DrawingRequest,
					input logic [7:0] left_bottom_hole_RGB,
			//added hole on middle top
					input logic middle_bottom_hole_DrawingRequest,
					input logic [7:0] middle_bottom_hole_RGB,
			//added hole on middle top
					input logic right_bottom_hole_DrawingRequest,
					input logic [7:0] right_bottom_hole_RGB,
					
			//Gifts
					input logic more_score_gift_DR,
					input logic [7:0] more_score_gift_RGB,
			
					input logic less_score_gift_DR,
					input logic [7:0] less_score_gift_RGB,		
					
					input logic slow_speed_gift_DR,
					input logic [7:0] slow_speed_gift_RGB,
		
					input logic fast_speed_gift_DR,
					input logic [7:0] fast_speed_gift_RGB,
			

			
		  ////////////////////////
		  // background 
				//baddies
					input logic boss_left_DR,
					input logic [7:0] boss_leftRGB,
					input logic boss_right_DR,
					input logic [7:0] boss_rightRGB,
					input logic boss_DR,
					input logic [7:0] bossRGB,
					input logic cute_DR,
					input logic [7:0] cuteRGB,
				//clouds
				   input logic cloud_1_DR,
					input logic [7:0] cloud_1RGB,
					input logic cloud_2_DR,
					input logic [7:0] cloud_2RGB,
					input logic cloud_3_DR,
					input logic [7:0] cloud_3RGB,
					input logic cloud_4_DR,
					input logic [7:0] cloud_4RGB,					
				//BG
					input		logic	[7:0] backGroundRGB, 
			  //output
				   output	logic	[7:0] RGBOut
);

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
			RGBOut	<= 8'b0;
	end
	
	else begin

 // add logic for box here 
		if(open_screen_draw == 1'b1)
				RGBOut <= RGB_open_screen;
				
		else if (end_screen_winner_draw == 1'b1)
					RGBOut <= RGB_end_screen_winner;
					
		else if (end_screen_loser_draw == 1'b1)
					RGBOut <= RGB_end_screen_loser;	
					
		else if (power_label_draw == 1'b1)
					RGBOut <= RGB_power_label;
					
		else if (score_label_draw == 1'b1)
					RGBOut <= RGB_score_label;
					
		else if (cursor_DrawingRequest == 1'b1)
					RGBOut <= cursorRGB;
			
		else if (level_DR == 1'b1)
					RGBOut <= levelRGB;
					
		else if (level_number_DR == 1'b1)
					RGBOut <= level_numberRGB;
					
		else if (white_ball_DrawingRequest == 1'b1)
				RGBOut <= white_ballRGB;

		else if (red_ball_DrawingRequest == 1'b1)
				RGBOut <= red_ballRGB;
				
		else if (black_ball_DrawingRequest == 1'b1)
				RGBOut <= black_ballRGB;
				
		else if (right_bot_target_DR == 1'b1)
				RGBOut <= right_bot_targetRGB;
				
		else if (left_top_target_DR == 1'b1)
				RGBOut <= left_top_targetRGB;
				
		else if (middle_bot_target_DR == 1'b1)
				RGBOut <= middle_bot_targetRGB;
				
		else if (right_top_target_DR == 1'b1)
				RGBOut <= right_top_targetRGB;
				
		else if (left_bot_target_DR == 1'b1)
				RGBOut <= left_bot_targetRGB;
				
		else if (middle_top_target_DR == 1'b1)
				RGBOut <= middle_top_targetRGB;	
		//holes
		else if (left_top_hole_DrawingRequest == 1'b1)
				RGBOut <= left_top_hole_RGB;
		
		else if (middle_top_hole_DrawingRequest == 1'b1)
				RGBOut <= middle_top_hole_RGB;
		
		else if (right_top_hole_DrawingRequest == 1'b1)
				RGBOut <= right_top_hole_RGB;
				
		else if (left_bottom_hole_DrawingRequest == 1'b1)
				RGBOut <= left_bottom_hole_RGB;
				
		else if (middle_bottom_hole_DrawingRequest == 1'b1)
				RGBOut <= middle_bottom_hole_RGB;
				
		else if (right_bottom_hole_DrawingRequest == 1'b1)
				RGBOut <= right_bottom_hole_RGB;
				
		else if (number_DrawingRequest == 1'b1)
				RGBOut <= numberRGB;
				
		else if (minus_DrawingRequest == 1'b1)
				RGBOut <= minusRGB;
				
		else if (power_DrawingRequest == 1'b1)
				RGBOut <= powerRGB;
		//Gifts
		else if (slow_speed_gift_DR == 1'b1)
				RGBOut <= slow_speed_gift_RGB;
				
		else if (fast_speed_gift_DR == 1'b1)
				RGBOut <= fast_speed_gift_RGB;
				
		else if (more_score_gift_DR == 1'b1)
				RGBOut <= more_score_gift_RGB;
				
		else if (less_score_gift_DR == 1'b1)
				RGBOut <= less_score_gift_RGB;
				
		else if (boss_left_DR == 1'b1)
				RGBOut <= boss_leftRGB;
				
		else if (boss_right_DR == 1'b1)
				RGBOut <= boss_rightRGB;
				
		else if (boss_DR == 1'b1)
				RGBOut <= bossRGB;
				
		else if (cute_DR == 1'b1)
				RGBOut <= cuteRGB;	
	
		else if (cloud_1_DR == 1'b1)
				RGBOut <= cloud_1RGB;
				
		else if (cloud_2_DR == 1'b1)
				RGBOut <= cloud_2RGB;
				
		else if (cloud_3_DR == 1'b1)
				RGBOut <= cloud_3RGB;
				
		else if (cloud_4_DR == 1'b1)
				RGBOut <= cloud_4RGB;
				

		else 
			RGBOut <= backGroundRGB ; // last priority 
		end ; 
	end

endmodule


