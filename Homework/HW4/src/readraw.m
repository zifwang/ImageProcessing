function G = readraw(filename,height,width)
%readraw - read RAW format grey scale image of square size into matrix G
% Usage:	G = readraw(filename)

	disp(['	Retrieving Image ' filename ' ...']);

	% Get file ID for file
	fid=fopen(filename,'rb');

	% Check if file exists
	if (fid == -1)
	  	error('can not open input image file press CTRL-C to exit \n');
	  	pause
	end

	% Get all the pixels from the image
	pixel = fread(fid, inf, 'uchar');

	% Close file
	fclose(fid);

	% Calculate length/width, assuming image is square
% 	[Y X]=size(pixel);
	Size=(height*width);
    

	% Construct matrix
	G = zeros(width,height);

	% Write pixels into matrix
	G(1:Size) = pixel(1:Size);
    
    G = uint8(G);

	% Transpose matrix, to orient it properly
	G = G';
end %function