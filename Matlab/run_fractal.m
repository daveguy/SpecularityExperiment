%
%  run_fractal.m
%
%  Generates an image with a 1/f amplitude spectra
%  where f = sqrt( fx * fx + fy * fy)  is the spatial frequency.
%  This is a technical trick that makes 'fractal' images with constant
%  power in each octave band.   
%  It is sometimes called 'spectral synthesis'.   

clear

N = 2048;    %  size of image

%  if you run this script many times, then you don't
%  want to run mk_oneoverf over and over again.

mk_oneoverf

%  Rather than adding a bunch of sinusoids together directly, 
%  we can set the Fourier transform 

I = real( ifft2( fft2( rand(N))  .* one_over_f ));

figure;
colormap gray;
imagesc(I)
axis square