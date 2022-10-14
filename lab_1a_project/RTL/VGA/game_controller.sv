// game controller dudy Febriary 2020
// (c) Technion IIT, Department of Electrical Engineering 2021 
//updated --Eyal Lev 2021


module    game_controller    (    
    		//system
			input	logic	clk,
			input	logic	resetN,
			input	logic	startOfFrame, 
		
			// inputs for cursor and balls
			input	logic	drawing_request_cursor,
			input	logic	drawing_request_white_ball,
			input logic drawing_request_red_ball,
			input logic drawing_request_black_ball,
			
			//inputs for holes
			input	logic	drawing_request_brackets,
			input	logic	drawing_request_left_top_hole,
			input	logic	drawing_request_middle_top_hole,
			input	logic	drawing_request_right_top_hole,
			input	logic	drawing_request_left_bot_hole,
			input	logic	drawing_request_middle_bot_hole,
			input	logic	drawing_request_right_bot_hole,
			
			input	logic	drawing_request_fast_speed_gift,
			input	logic	drawing_request_slow_speed_gift,
			input	logic	drawing_request_more_score_gift,
			input	logic	drawing_request_less_score_gift,
			input logic can_cont,
			
			input logic [3:0] keyPad, //shortcut
			input logic hit_occured,
			input logic key_ENTER_is_pressed,
			
			
			//outputs
			output logic collision, // active in case of collision between two objects
			output logic collision_white_ball_brackets,
			output logic collision_white_ball_hole,
			
			output logic collision_red_ball_brackets,
			output logic collision_red_ball_hole,

			output logic collision_black_ball_brackets,
			output logic collision_black_ball_hole,
			
			output logic collision_red_ball_white_ball,
			output logic collision_black_ball_white_ball,
			output logic collision_black_ball_red_ball,
			
			//singelHitPulse
			output logic SingleHitPulse, // critical code, generating A single pulse in a frame 
			output logic minus_on,
			output logic draw_target_right_bot_out,
			output logic draw_target_left_top_out,
			output logic draw_target_middle_bot_out,
			output logic draw_target_right_top_out,
			output logic draw_target_left_bot_out,
			output logic draw_target_middle_top_out,
			output logic draw_slow_speed_gift,
			output logic draw_fast_speed_gift,
			output logic draw_more_score_gift,
			output logic draw_less_score_gift,
			output logic collision_white_ball_fast_speed_gift,
			output logic collision_white_ball_slow_speed_gift,
			output logic collision_red_ball_fast_speed_gift,
			output logic collision_red_ball_slow_speed_gift,
			output logic collision_black_ball_fast_speed_gift,
			output logic collision_black_ball_slow_speed_gift,			
			output logic collision_any_ball_any_gift,
			output logic enable_black_ball,
			output logic open_screen,
			output logic end_screen_winner,
			output logic end_screen_loser,
			output logic [3:0] current_level,
			
      //output of sm
			output logic [3:0] final_counter
);

assign collision_white_ball_brackets = ( drawing_request_white_ball &&  drawing_request_brackets );
assign collision_white_ball_hole = (drawing_request_white_ball && (drawing_request_left_top_hole || drawing_request_middle_top_hole ||
															drawing_request_right_top_hole || drawing_request_left_bot_hole ||
															drawing_request_middle_bot_hole || drawing_request_right_bot_hole));

	
assign collision_red_ball_brackets	= ( drawing_request_red_ball &&  drawing_request_brackets );
assign collision_red_ball_hole = (drawing_request_red_ball && (drawing_request_left_top_hole || drawing_request_middle_top_hole ||
															 drawing_request_right_top_hole || drawing_request_left_bot_hole ||
															 drawing_request_middle_bot_hole || drawing_request_right_bot_hole));
															 
assign collision_black_ball_brackets	= ( drawing_request_black_ball &&  drawing_request_brackets );
assign collision_black_ball_hole = (drawing_request_black_ball && (drawing_request_left_top_hole || drawing_request_middle_top_hole ||
															 drawing_request_right_top_hole || drawing_request_left_bot_hole ||
															 drawing_request_middle_bot_hole || drawing_request_right_bot_hole));
															
															
assign collision_red_ball_white_ball = (drawing_request_white_ball &&  drawing_request_red_ball);
assign collision_black_ball_white_ball = (drawing_request_black_ball &&  drawing_request_white_ball);
assign collision_black_ball_red_ball = (drawing_request_black_ball &&  drawing_request_red_ball);

assign collision_white_ball_fast_speed_gift = (drawing_request_white_ball &&  drawing_request_fast_speed_gift);
assign collision_white_ball_slow_speed_gift = (drawing_request_white_ball &&  drawing_request_slow_speed_gift);
assign collision_red_ball_fast_speed_gift = (drawing_request_red_ball &&  drawing_request_fast_speed_gift);
assign collision_red_ball_slow_speed_gift = (drawing_request_red_ball &&  drawing_request_slow_speed_gift);
assign collision_black_ball_fast_speed_gift = (drawing_request_black_ball &&  drawing_request_fast_speed_gift);
assign collision_black_ball_slow_speed_gift = (drawing_request_black_ball &&  drawing_request_slow_speed_gift);

assign collision_any_ball_any_gift = ((drawing_request_white_ball || drawing_request_red_ball || drawing_request_black_ball) && 
													(drawing_request_fast_speed_gift || drawing_request_slow_speed_gift ||
													draw_more_score_gift || draw_less_score_gift));
													
logic collision_red_ball_right_bot_hole; 
logic collision_red_ball_left_top_hole;
logic	collision_red_ball_middle_bot_hole;
logic	collision_red_ball_right_top_hole;
logic	collision_red_ball_left_bot_hole;
logic	collision_red_ball_middle_top_hole;

logic collision_white_ball_more_score_gift; 
logic collision_white_ball_less_score_gift;
logic collision_red_ball_more_score_gift;
logic collision_red_ball_less_score_gift;
logic collision_black_ball_more_score_gift;
logic collision_black_ball_less_score_gift;


logic flag ; // a semaphore to set the output only once per frame / regardless of the number of collisions 
always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin 
		flag	<= 1'b0;
		SingleHitPulse <= 1'b0 ;
	end 
	else begin 

			SingleHitPulse <= 1'b0 ; // default 
			if(startOfFrame) begin
				flag <= 1'b0 ; // reset for next time 
				/*if (drawing_request_white_ball &&  drawing_request_red_ball) begin
					flag <= 1'b0 ;
					collision_red_ball_white_ball <= 1;
				end*/
//		change the section below  to collision between two objects

		end 
	end 
end


/////////////////********************* LEVELS STATE MACHINE *********************/////////////////
enum logic [10:0] {s_idle, s_level_1, s_level_2, s_level_3, s_winner_end, s_loser_end} game_ps, game_ns; // state machine
enum logic [3:0] {draw_target_right_bot, draw_target_left_top, draw_target_middle_bot, draw_target_right_top, draw_target_left_bot,
				draw_target_middle_top, kill_target, add_score, substruct_score} draw_target_prev, draw_target_ps , draw_target_next; // state machine
enum logic [2:0] {gift_idle, slow_speed_gift, fast_speed_gift, more_score_gift, less_score_gift} gift_ps, gift_next;

logic [3:0] score_counter_ps, score_counter_next;
logic minus_ps, minus_next;
int increased_score = 0, decreased_score = 0;
int start_of_frame_on = 1;
int can_change_gift_state = 1;
int wait_for_next_can_cont = 0;
int entered_level_3 = 0;
int score_limit = 2;
	//--------------------------------------------------------------------------------------------
	//  1.  syncronous code,  executed once every clock to update the current state 
		
	always_ff @(posedge clk or negedge resetN) // State machine logic ////
	   begin
		   
	   if ( !resetN ) begin // Asynchronic reset, initialize the state machine 
			game_ps <= s_idle;
			score_counter_ps <= 0;
			draw_target_ps <= draw_target_right_bot;
			draw_target_prev <= draw_target_right_bot;
			minus_ps <= 0;
			gift_ps <= gift_idle;
			score_limit <= 2;
			can_change_gift_state <= 1;
			entered_level_3 <= 0;
		end // asynch
		
		else begin 		   // Synchronic logic of the state machine; update score when hit a hole
			if(((score_counter_ps != score_counter_next) && start_of_frame_on) || (game_ps != game_ns)) begin
				score_counter_ps <= score_counter_next;
				start_of_frame_on = 0;
			end
			if(startOfFrame == 1) begin //to allow to count score only onces
				start_of_frame_on = 1;	
			end
			game_ps <= game_ns;		  
			minus_ps <= minus_next;
			if(draw_target_ps != draw_target_next) begin
				draw_target_prev <= draw_target_ps;
			end
			draw_target_ps <= draw_target_next;
			
			//Gifts show control
			if((gift_ps != gift_next) || collision_any_ball_any_gift || can_cont) begin
				can_change_gift_state <= 0;
			end
			
			if(!can_cont) begin
				wait_for_next_can_cont <= 1;
			end
			
			if(wait_for_next_can_cont && can_cont) begin // need to wait for next can_cont
				can_change_gift_state <= 1;
			end
			
			if(can_change_gift_state && can_cont) begin
				gift_ps <= gift_next;
				wait_for_next_can_cont <= 0;
				can_change_gift_state <= 0;
				increased_score <= 0;
				decreased_score <= 0;
			end	
			//flags to count extra or less score only once
			if(collision_white_ball_less_score_gift || collision_red_ball_less_score_gift || collision_white_ball_more_score_gift || 
				collision_red_ball_more_score_gift || collision_black_ball_more_score_gift || collision_black_ball_less_score_gift) begin
				if((score_counter_ps != score_counter_next)) begin
					increased_score <= 1;
					decreased_score <= 1;
				end
			end
			//notify that we reached level 3
			if(game_ps == s_level_3) begin
				score_limit <= 3;
				entered_level_3 <= 1;
			end
		end 
			
	end // always_ff state machine ///////////////////////////////
	
		
	//--------------------------------------------------------------------------------------------
	//  2.  asynchornous code : logic defining what is the next state 	
		
	always_comb // Update the next state  /////////////////////////
		begin
		// 0. Set default operation values ;
			game_ns = game_ps;
			score_counter_next = score_counter_ps;	
			minus_next = minus_ps;
			draw_target_next = draw_target_ps;
			gift_next = gift_ps;
			collision_red_ball_right_bot_hole = (drawing_request_red_ball && drawing_request_right_bot_hole);
			collision_red_ball_left_top_hole = (drawing_request_red_ball && drawing_request_left_top_hole);
			collision_red_ball_middle_bot_hole = (drawing_request_red_ball && drawing_request_middle_bot_hole);
			collision_red_ball_right_top_hole = (drawing_request_red_ball && drawing_request_right_top_hole);
			collision_red_ball_left_bot_hole = (drawing_request_red_ball && drawing_request_left_bot_hole);
			collision_red_ball_middle_top_hole = (drawing_request_red_ball && drawing_request_middle_top_hole);
			
			collision_white_ball_more_score_gift = (drawing_request_white_ball && drawing_request_more_score_gift) ;
			collision_white_ball_less_score_gift = (drawing_request_white_ball && drawing_request_less_score_gift) ;
			collision_red_ball_more_score_gift = (drawing_request_red_ball && drawing_request_more_score_gift);
			collision_red_ball_less_score_gift = (drawing_request_red_ball && drawing_request_less_score_gift);
			collision_black_ball_more_score_gift = (drawing_request_black_ball && drawing_request_more_score_gift);
			collision_black_ball_less_score_gift = (drawing_request_black_ball && drawing_request_less_score_gift);
			open_screen = 1'b0;
			end_screen_winner = 1'b0;
			end_screen_loser = 1'b0;
			
			case (game_ps) 
				s_idle : begin
					open_screen = 1'b1;
					if(key_ENTER_is_pressed) begin
						game_ns = s_level_1;
					end
					else begin
						game_ns = s_idle;
					end
				end
				s_level_1 : begin
					if ((score_counter_ps == 2 && minus_ps == 0) || keyPad == 4'h5) begin //conditions for level 2 and a cheat
						score_counter_next = 0;
						minus_next = 0;
						draw_target_next = kill_target;
						gift_next = gift_idle;
						game_ns = s_level_2; 
					end
					else begin
						case (draw_target_ps)
							//right bot target -- 1
							draw_target_right_bot : begin
								if(collision_red_ball_right_bot_hole) begin
									draw_target_next = add_score;
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score;
								end
								else begin
									draw_target_next = draw_target_right_bot;
								end
							end
								
							 //left top target -- 2
							draw_target_left_top : begin 
								if(collision_red_ball_left_top_hole) begin 
									draw_target_next = add_score;
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score;
								end 
								else begin
									draw_target_next = draw_target_left_top;
								end
							end 
							
							//middle bot target -- 3
							draw_target_middle_bot : begin
								if(collision_red_ball_middle_bot_hole) begin 
									draw_target_next = add_score; 
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score; 
								end
								else begin
									draw_target_next = draw_target_middle_bot;
								end
							end 

							//right top target -- 4
							draw_target_right_top : begin 
								if(collision_red_ball_right_top_hole) begin 
									draw_target_next = add_score; 
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score; 
								end
								else begin
									draw_target_next = draw_target_right_top;
								end
							end 

							//left bot target -- 5
							draw_target_left_bot : begin 
								if(collision_red_ball_left_bot_hole) begin 
									draw_target_next = add_score;
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score;
								end 
								else begin
									draw_target_next = draw_target_left_bot;
								end
							end 					
							
							//middle top target -- 6
							draw_target_middle_top : begin 
								if(collision_red_ball_middle_top_hole) begin 
									draw_target_next = add_score;
								end
								else if(collision_white_ball_hole) begin
									draw_target_next = substruct_score;
								end 
								else begin
									draw_target_next = draw_target_middle_top;
								end
							end
							//adding score state
							add_score: begin
									if(minus_ps == 1) begin//neg
										score_counter_next = score_counter_ps - 1; //Go towards zero
										if(score_counter_next == 0) begin //If reached zero turn off minus
											minus_next = 0;
										end
										else begin // if not reached zero, keep minus on
											minus_next = 1;
										end
									end // closing neg
									else begin//pos, no need for minus
										score_counter_next = score_counter_ps + 1;
										minus_next = 0;
									end // closing pos
									case(draw_target_prev)
										draw_target_right_bot : begin
											draw_target_next = draw_target_left_top;
										end
										draw_target_left_top : begin
											draw_target_next = draw_target_middle_bot;
										end
										draw_target_middle_bot : begin
											draw_target_next = draw_target_right_top;
										end
										draw_target_right_top : begin
											draw_target_next = draw_target_left_bot;
										end
										draw_target_left_bot : begin
											draw_target_next = draw_target_middle_top;
										end
										draw_target_middle_top : begin
											draw_target_next = draw_target_right_bot;
										end
									endcase
							end
							
							//substructing score state
							substruct_score: begin
									if(minus_ps == 0) begin//pos
										if(score_counter_ps == 0) begin //if starting to count from zero
											score_counter_next = 1;
											minus_next = 1;
										end
										else begin // positive number that is not zero
											score_counter_next = score_counter_ps - 1;
											minus_next = 0;
										end
									end // closing if
									else begin//neg
										score_counter_next = score_counter_ps + 1;
										minus_next = 1;
									end
									draw_target_next = draw_target_prev;
							end 
						endcase
						
					end // closing else
				end // closing s_level_1
				
				//level2
				s_level_2: begin
					if((score_counter_ps == score_limit && minus_ps == 0 && !entered_level_3) || keyPad == 0) begin //conditions for level 3 and a cheat
						score_counter_next = 0;
						minus_next = 0;
						gift_next = gift_idle;
						draw_target_next = kill_target;
						game_ns = s_level_3; 
					end
					else if(score_counter_ps == score_limit && minus_ps == 0 && entered_level_3) begin
						game_ns = s_winner_end;
					
					end				
					else if(collision_black_ball_hole) begin
						game_ns = s_loser_end;
					
					end
					else begin
							draw_target_next = kill_target;
							if(collision_red_ball_hole) begin // change hole
								if(minus_ps == 1) begin//neg
									score_counter_next = score_counter_ps - 1;
									if(score_counter_next == 0) begin
										minus_next = 0;
									end
									else begin
										minus_next = 1;
									end
								end
								else begin//pos
									score_counter_next = score_counter_ps + 1;
									minus_next = 0;

								end
							end
							else if(collision_white_ball_hole) begin
								if(minus_ps == 0) begin//pos
									if(score_counter_ps == 0) begin
										score_counter_next = 1;
										minus_next = 1;
									end
									else begin
										score_counter_next = score_counter_ps - 1;
										minus_next = 0;
									end
								end // closing if
								else begin//neg
									score_counter_next = score_counter_ps + 1;
									minus_next = 1;
								end
							end // closing else if	
							
							//GIFTS
							else if(gift_ps == gift_idle) begin;
								if(can_change_gift_state) begin
									gift_next = slow_speed_gift;
								end
							end
							
							else if(gift_ps == slow_speed_gift) begin
								if(can_change_gift_state) begin
									gift_next = more_score_gift;
								end							
							end
							
							else if(gift_ps == fast_speed_gift) begin
								if(can_change_gift_state) begin
									gift_next = less_score_gift;
								end
							end
							
							
							else if(gift_ps == more_score_gift) begin; //need to do an adding point logic only once
								if((collision_white_ball_more_score_gift || collision_red_ball_more_score_gift || collision_black_ball_more_score_gift) && !increased_score) begin
									if(minus_ps == 1) begin//neg
										score_counter_next = score_counter_ps - 1;
										if(score_counter_next == 0) begin
											minus_next = 0;
										end
										else begin
											minus_next = 1;
										end
									end
									else begin//pos
										score_counter_next = score_counter_ps + 1;
										minus_next = 0;
									end
								end
								if(can_change_gift_state) begin
									gift_next = fast_speed_gift;
								end
							end
							
							else if(gift_ps == less_score_gift) begin //need to do a substructing point logic only once
								if((collision_white_ball_less_score_gift || collision_red_ball_less_score_gift ||collision_black_ball_less_score_gift) && !decreased_score) begin
									if(minus_ps == 0) begin//pos
										if(score_counter_ps == 0) begin
											score_counter_next = 1;
											minus_next = 1;
										end
										else begin
											score_counter_next = score_counter_ps - 1;
											minus_next = 0;
										end
									end // closing if
									else begin//neg
										score_counter_next = score_counter_ps + 1;
										minus_next = 1;
									end;
								end
								if(can_change_gift_state) begin
									gift_next = gift_idle;
								end
							end
							
							
							else begin	//idle state
								gift_next = gift_idle;
							end
						game_ns = s_level_2; 
					end
				end
				//level 3
				s_level_3: begin //return to level 3 but with updated score limit and enable the black ball
						score_counter_next = 0;
						minus_next = 0;
						gift_next = gift_idle;
						draw_target_next = kill_target;
						game_ns = s_level_2; 
				end
				s_winner_end: begin
					end_screen_winner = 1'b1;
					end_screen_loser = 1'b0;
					if(key_ENTER_is_pressed) begin
						game_ns = s_idle;
					end
					else begin
						game_ns = s_winner_end;
					end
				end
				
				s_loser_end: begin
					end_screen_winner = 1'b0;
					end_screen_loser = 1'b1;
					if(key_ENTER_is_pressed) begin
						game_ns = s_idle;
					end
					else begin
						game_ns = s_loser_end;
					end
				end
			endcase	
	end
	//assign output lines
	assign final_counter = score_counter_ps;
	assign minus_on = minus_ps;
	assign draw_target_right_bot_out = (draw_target_ps == draw_target_right_bot) ? 1 : 0;
	assign draw_target_left_top_out = (draw_target_ps == draw_target_left_top) ? 1 : 0;
	assign draw_target_middle_bot_out = (draw_target_ps == draw_target_middle_bot) ? 1 : 0;
	assign draw_target_right_top_out = (draw_target_ps == draw_target_right_top) ? 1 : 0;
	assign draw_target_left_bot_out = (draw_target_ps == draw_target_left_bot) ? 1 : 0;
	assign draw_target_middle_top_out = (draw_target_ps == draw_target_middle_top) ? 1 : 0;
	
	assign draw_slow_speed_gift = (gift_ps == slow_speed_gift) ? 1 : 0;
	assign draw_fast_speed_gift = (gift_ps == fast_speed_gift) ? 1 : 0;
	assign draw_more_score_gift = (gift_ps == more_score_gift) ? 1 : 0;
	assign draw_less_score_gift = (gift_ps == less_score_gift) ? 1 : 0;

	assign enable_black_ball = entered_level_3;
	assign current_level = (entered_level_3) ? 3 : game_ps;
	
	
endmodule