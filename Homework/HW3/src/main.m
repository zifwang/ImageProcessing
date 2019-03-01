%% Author: Zifan Wang
%% Homework3: problem 1: b,c main function
%%
function main(image_1_path,image_1_height,image_1_width,image_2_path,image_2_height,image_2_width)
    %%
    image1 = readraw(image_1_path,image_1_height,image_1_width);
    mod_image_1 = spatialWarp(image1,0.25);
    figure;
    imshow(uint8(mod_image_1));
    %%
    image2 = readraw(image_2_path,image_2_height,image_2_width);
    mod_image_2 = lensDistortionCorrection(image2);
    figure;
    imshow(uint8(mod_image_2));
end