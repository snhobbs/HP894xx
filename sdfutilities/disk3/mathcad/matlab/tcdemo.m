echo on
clc
%	This demonstration will show you how to digitally filter
%	HP3566/67A time capture data using PC-MATLAB Signal
%	Processing Toolbox.
%	
%	This demonstration assumes that a 4 channel time capture 
%	file with a span of 12800 Hz span has been acquired, and 
%	has been saved as 'matdemo' in the \analyzer\tc directory. 
%
%	NOTE: The data in this file will be overwritten by the 
%	refiltered data.
%
%	If you don't have an appropriate time capture file, 
%	hit CTRL-C, else hit any other key to continue...
pause 
clc
%	First, lets create a second order Butterworth high pass 
%	filter with a cutoff frequency of 2048 Hz.

w1 = [.125];
[b,a] = butter(2,w1,'high');

%	Now that the coefficents are calculated, lets plot the
%	transfer function.

pause % Strike any key for plot.

[h,w] = freqz(b,a,128);
f = w*16384/pi;
loglog(f,abs(h),'g'),title('second order Butterworth high pass'),..
xlabel('Frequency(Hz)'),ylabel('Magnitude'),grid,pause
clc

% 	Now lets filter the third channel of the HP3566/67A time 
%	capture file using the filter coefficients created above 
%	and the 'filtrsdf' command.

filtrsdf(b,a,'\analyzer\tc\matdemo.tim',0,2,0);

% Channel 3 has been sucessfully digitally filtered.
%
pause % Strike any key to continue.
clc

%	Lets create a fourth order Chebyshev bandpass filter with
%	0.5 dB of ripple from 4096 Hz to 8192 Hz.
w1 = [.25,.50];
[b,a] = cheby1(4,0.5,w1);

%	Now that the coefficents are calculated, lets plot the
%	transfer function.

pause % Strike any key for plot.

[h,w] = freqz(b,a,128);
f = w*16384/pi;
loglog(f,abs(h),'g'),title('fourth order Chebyshev bandpass'),..
xlabel('Frequency(Hz)'),ylabel('Magnitude'),grid,pause
clc

% Now lets filter the fourth channel of the HP3566/67A time capture 
% file using the filter coefficients created above and the 
% 'filtrsdf' command.

filtrsdf(b,a,'\analyzer\tc\matdemo.tim',0,3,0);

clc
%	The file 'matdemo' now has digitally filtered the data 
%	in the third and fourth channels. To process this data,
%	do the following:
%
%		1] exit MATLAB
%		2] fire up HP3566/67A
%		3] load FRF measurement
%		4] recall time capture file 'matdemo'
%		5] start the measurement
%
%	If you want to compare this data with the original MATLAB
%	filter shapes, do the following:
%
%		1] Turn on stable averaging
%		2] start the measurement
%		3] At completion, save data into 'matfrf'
%		4] exit HP3566/67A
%		5] fire up MATLAB
%		6] run tcdemo2

pause	% This ends the digital filtering demo. Hit any key to continue.
