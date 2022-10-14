/// (c) Technion IIT, Department of Electrical Engineering 2021 
//-- This module  generate the correet prescaler tones for a single ocatave 

//-- Dudy Feb 12 2019 
//-- Eyal Lev --change values to 25MHz   Feb 2021

module	ToneDecoder	(	
					input	logic [3:0] tone, 
					output	logic [9:0]	preScaleValue
		);

logic [15:0] [9:0]	preScaleValueTable = { 

//---------------VALUES for 50MHz------------------------

 //10'h2EA,   decimal =746.55      Hz =261.62  do    50_000_000/256/<FREQ_Hz>
 //10'h2C0,   decimal =704.64      Hz =277.18  doD
 //10'h299,   decimal =665.09      Hz =293.66  re
 //10'h273,   decimal =627.77      Hz =311.12  reD
 //10'h250,   decimal =592.53      Hz =329.62  mi
 //10'h22F,   decimal =559.28      Hz =349.22  fa
 //10'h20F,   decimal =527.87      Hz =370  faD
 //10'h1F2,   decimal =498.24      Hz =392  sol
 //10'h1D6,   decimal =470.29      Hz =415.3  solD
 //10'h1BB,   decimal =443.89      Hz =440  La
 //10'h1a3,   decimal =418.98      Hz =466.16  laD
 //10'h18b} ; decimal =395.46      Hz =493.88  si
//10'h345,    decimal =837.96      Hz =233.08  laD
//10'h316} ;  decimal =790.93      Hz =246.94  si

//---------------VALUES for 25MHz------------------------

10'h175,   // decimal =373.27      Hz =261.62  do    25_000_000/256/<FREQ_Hz>
10'h160,   // decimal =352.32      Hz =277.18  doD
10'h14C,   // decimal =332.54      Hz =293.66  re
10'h139,   // decimal =313.88      Hz =311.12  reD
10'h128,   // decimal =296.26      Hz =329.62  mi
10'h117,   // decimal =279.64      Hz =349.22  fa
10'h107,   // decimal =263.93      Hz =370    faD
10'h0F9,   // decimal =249.12      Hz =392    sol
10'h0EB,   // decimal =235.14      Hz =415.3  solD
10'h0DD,   // decimal =221.94      Hz =440    La
10'h0D1,   // decimal =209.49      Hz =466.16  laD
10'h0C5} ; // decimal =197.73      Hz =493.88  si
//10'h1A2,   // decimal =418.98      Hz =233.08  laD
//10'h18B} ; // decimal =395.46      Hz =246.94  si

assign 	preScaleValue = preScaleValueTable [tone] ; 

endmodule






























