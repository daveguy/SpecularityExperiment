%
%  mk_oneoverf.m
%
%  Compute a function 1/f^alpha 
%  which will be used to set the amplitude spectrum falloff.
%
%  You can set the minimum and maximum frequencies, if you
%  want to make it bandpass, lowpass, or highpass.

alpha = 1;

minK = 3;
maxK = 15;     %  N/2 is Nyquist.

one_over_f = zeros(N);

for kx = 1:N           % frequency indices
  for ky = 1:N

   k = [kx - (N/2 + 1), ky - (N/2 + 1)]';
   if (k'*k >= minK*minK) && (k'*k <= maxK*maxK)
    
     kTk =  pow2(alpha* log2(k' * k));	
          
     one_over_f(kx,ky) = 1 / sqrt(kTk);
   else
     one_over_f(kx,ky) =  0;
   end 
 
  end;
end;

%  Need to put the (0,0) dc frequency back at the array index (1,1)
%  rather than at the center of the array.

one_over_f = fftshift(one_over_f);

clear t kTk kx ky t


