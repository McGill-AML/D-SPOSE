%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FUNCTION NAME:        load_propagation
%
% DESCRIPTION:          This function will load data from a D-SPOSE
%                       output file.
%
% AUTHOR:               Luc Sagnieres
% DATE:                 August 10, 2015
% VERSION:              1
%
% INPUT:                - loc: location of input file
%
% OUTPUT:               - prop: propagation array
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function prop = load_propagation(loc)

% Open file
fileID = fopen(loc,'r');

s=0;
% Read file
while (~feof(fileID))
    temp = fgetl(fileID);
    if ~isempty(temp)
        if ~strcmp(temp(1),'#')
            s = s+1;
        end
    end
end

% Close file
fclose(fileID);

prop = zeros(s,14);

% Open file
fileID = fopen(loc,'r');

% Read file
j=1;
while (~feof(fileID))
    temp = fgetl(fileID);
    if ~isempty(temp)
        if ~strcmp(temp(1),'#')
            prop(j,:) = str2num(temp);
            j=j+1;
        end
    end
end

% Close file
fclose(fileID);

end