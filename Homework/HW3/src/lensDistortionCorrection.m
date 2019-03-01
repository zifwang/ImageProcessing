function corrected_image = lensDistortionCorrection(image)

    function pixelVal = bilinearInterpolation(img,p)
        % bilinearInterpolation:
        % Argument: image, point(x,y)
        % return: modify pixel value
        x=fix(p(1));
        y=fix(p(2));
        pixelVal=(img(x,y)*(y+1-p(2))*(x+1-p(1))+img(x,y+1)*(x+1-p(1))*(p(2)-y)+img(x+1,y)*(p(1)-x)*(y+1-p(2))+img(x+1,y+1)*(p(1)-x)*(p(2)-y));
    end

    function [x,y] = transform(p,R,fx,fy)
        % Implement the relationship between the actual image and its distortion in the camera coordinate system
        % x_d = x(1 + k_1 * r^2 + k_2 * r_4 + k_3 * r_6 )
        % y_d = y(1 + k_1 * r^2 + k_2 * r_4 + k_3 * r_6 )
        % r^2 = x^2 + y^2
        % Argument: point(x,y) -> undistorted pixel location
        % Return: x_d, y_d: undistorted pixel location
        if(fx >= fy)
            f_max = fx;
            f_min = fy;
        else
            f_max = fy;
            f_min = fx;
        end
        
        k_1 = -0.3536;
        k_2 = 0.1730;
        k_3 = 0;
        r = sqrt(p(1)^2+p(2)^2)/f_min;
        R = R/f_max;
        x = p(1)*(1 + k_1*r^2 + k_2*r^4 + k_3*r^6)/(1 + k_1*R^2 + k_2*R^4 + k_3*R^6);
        y = p(2)*(1 + k_1*r^2 + k_2*r^4 + k_3*r^6)/(1 + k_1*R^2 + k_2*R^4 + k_3*R^6);
    end


    [imgHeight,imgWidth] = size(image);
    corrected_image = zeros(imgHeight,imgWidth);
    center_u = (1+imgHeight)/2;
    center_v = (1+imgWidth)/2;
    R = max(center_u,center_v);
    fx = 600;
    fy = 600;
    
    for i = 1:imgHeight
        for j = 1:imgWidth
            x_u_c = i - center_u;
            y_u_c = j - center_v;
            [x_d,y_d] = transform([x_u_c,y_u_c],R,fx,fy);
            if(x_d+center_u<1 || x_d+center_u >= imgHeight || y_d+center_v<1 || y_d+center_v>=imgWidth)
                continue;
            end
            corrected_image(i,j) = bilinearInterpolation(image,[x_d+center_u,y_d+center_v]);
        end
    end
end