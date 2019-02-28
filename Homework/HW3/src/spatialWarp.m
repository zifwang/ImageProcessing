function warpedImage = spatialWarp(image,prob)
    %% Author: Zifan Wang
    %% Implement the spatial warping for an image
    %% Argument: image: input image (the image to warp), arc: (an arc parameter)
    %% Return: an warped image
    %%
    %   get image's height & width
    [imgHeight,imgWidth] = size(image);
        % set output image
    warpedImage = zeros(imgHeight,imgWidth);
    %   get the center location
    center = [(1+imgHeight)/2,(1+imgWidth)/2];
    %   define reference point
    rf_p1 = [1,1] - center;                         % top left
    rf_p2 = [1,(1+imgWidth)/2] - center;            % top middle
    rf_p3 = [1,imgWidth] - center;                  % top right
    rf_p4 = [(1+imgHeight)/2,imgWidth] - center;    % right middle
    rf_p5 = [imgHeight,imgWidth] - center;          % bottom right
    rf_p6 = [imgHeight,(1+imgWidth)/2] - center;    % bottom middle
    rf_p7 = [imgHeight, 1] - center;                % bottom left
    rf_p8 = [(1+imgHeight)/2,1] - center;           % left middle
    rf_p9 = [(1+imgHeight)/2,(1+imgWidth)/2] - center; % middle point
    rf_p10 = [imgHeight*prob+1-prob,imgWidth*prob+1-prob] - center;
    rf_p11 = [imgHeight*prob+1-prob,imgWidth*(1-prob)+prob] -center;
    rf_p12 = [imgHeight*(1-prob)+prob,imgWidth*prob+1-prob] - center;
    rf_p13 = [imgHeight*(1-prob)+prob,imgWidth*(1-prob)+prob] - center;
    rf_p14 = [imgHeight*prob+1-prob,(1+imgWidth)/2] - center;
    rf_p15 = [(1+imgHeight)/2,imgWidth*prob+1-prob] - center;
    rf_p16 = [(1+imgHeight)/2,imgWidth*(1-prob)+prob] - center;
    rf_p17 = [imgHeight*(1-prob)+prob,(1+imgWidth)/2] - center;
    
    function mat = matrixConstruct(point1,point2,point3,point4,point5,point6)
        x = [point1(1),point2(1),point3(1),point4(1),point5(1),point6(1)];
        y = [point1(2),point2(2),point3(2),point4(2),point5(2),point6(2)];
        mat = [1,1,1,1,1,1;
               x;
               y;
               x.^2;
               x.*y;
               y.^2;
              ];
    end

    function pixelVal = bilinearInterpolation(img,p)
        x=fix(p(1));
        y=fix(p(2));
        pixelVal=(img(x,y)*(y+1-p(2))*(x+1-p(1))+img(x,y+1)*(x+1-p(1))*(p(2)-y)+img(x+1,y)*(p(1)-x)*(y+1-p(2))+img(x+1,y+1)*(p(1)-x)*(p(2)-y));
    end
        

    % get location
    % top
    new1 = [rf_p1',rf_p2',rf_p3',rf_p10',rf_p9',rf_p11']/matrixConstruct(rf_p1,rf_p14,rf_p3,rf_p10,rf_p9,rf_p11);
    % rigth
    new2 = [rf_p3',rf_p4',rf_p5',rf_p11',rf_p9',rf_p13']/matrixConstruct(rf_p3,rf_p16,rf_p5,rf_p11,rf_p9,rf_p13);
    % bottom
    new3 = [rf_p7',rf_p6',rf_p5',rf_p12',rf_p9',rf_p13']/matrixConstruct(rf_p7,rf_p17,rf_p5,rf_p12,rf_p9,rf_p13);
    % left
    new4 = [rf_p1',rf_p8',rf_p7',rf_p10',rf_p9',rf_p12']/matrixConstruct(rf_p1,rf_p15,rf_p7,rf_p10,rf_p9,rf_p12);

    for i = 1:imgHeight
        for j = 1:imgWidth
            p_x = i - center(1);
            p_y = j - center(2);
%             x_ori = 0;
%             y_ori = 0;
            trans = [1;p_x;p_y;p_x^2;p_x*p_y;p_y^2];
            
            % top part
            if(p_y > p_x && p_y < -p_x)
                t = new1*trans;
                x_ori = t(1);
                y_ori = t(2);
            %  right part
            elseif(p_y >= abs(p_x))
                t = new2*trans;
                x_ori = t(1);
                y_ori = t(2);
            % left part
            elseif(p_y <= -abs(p_x))
                t = new4*trans;
                x_ori = t(1);
                y_ori = t(2);
            else
                t = new3*trans;
                x_ori = t(1);
                y_ori = t(2);
            end
            
            if(x_ori+center(1) < 1 || x_ori+center(1) >= imgHeight || y_ori+center(2) < 1 || y_ori+center(2) >= imgWidth)
                continue;
            end
            
            warpedImage(i,j) = bilinearInterpolation(image,[x_ori+center(1),y_ori+center(2)]);
        end
    end
    
    
end

