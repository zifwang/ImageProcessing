clear

% %% set opts for training (see edgesTrain.m)
% opts=edgesTrain();                % default options (good settings)
% opts.modelDir='models/';          % model will be in models/forest
% opts.modelFnm='modelBsds';        % model name
% opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
% opts.useParfor=0;                 % parallelize if sufficient memory
% 
% %% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
% tic, model=edgesTrain(opts); toc; % will load model if already trained
% 
% %% set detection parameters (can set after training)
% model.opts.multiscale=1;          % for top accuracy set multiscale=1
% model.opts.sharpen=2;             % for top speed set sharpen=0
% model.opts.nTreesEval=5;          % for top speed set nTreesEval=1
% model.opts.nThreads=4;            % max number threads for evaluation
% model.opts.nms=1;                 % set to true to enable nms
% 
% %% evaluate edge detector on BSDS500 (see edgesEval.m)
% if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
I = imread('out_tiger_200_600.jpg');

% tic, E=edgesDetect(I,model); toc
% figure(1); im(I); figure(2); im(1-E);
%I = readraw('out_sobel_pig_005.raw',321,481);
I = single(I./255);



precision_mat = [];
recall_mat = [];
for i = 1:5
    filename = strcat('Pig',int2str(i));
    filename = strcat(filename,'.mat');
    [thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(I,filename);
    percision = cntP./sumP;
    recall = cntR./sumR;
    F = 2.*(percision.*recall)./(percision+recall);
    lThreshold = 0;
    max = 0;
    for j = 1:99
        if F(j) > max
            max = F(j);
            lThreshold = j;
        end
    end
    lPrecision = percision(j);
    precision_mat(i) = lPrecision;
    lRecall = recall(j);
    recall_mat(i) = lRecall;
end       
meanPre = mean(precision_mat)
meanReca = mean(recall_mat)
Fmean = 2*(meanPre*meanReca)/(meanPre+meanReca)