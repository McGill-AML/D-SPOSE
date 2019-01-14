%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FUNCTION NAME:        load_geometry
%
% DESCRIPTION:          This function will load a D-SPOSE geometry file and
%                       plot the surface geometry model with inward surface
%                       normals.
%
% AUTHOR:               Luc Sagnieres
% DATE:                 December 10, 2018
% VERSION:              1
%
% INPUT:                - loc: location of input file
%
% OUTPUT:               - surf: array containing surface geometry model
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function surf = load_geometry(loc)

% Open file
fileID = fopen(loc,'r');

s=0;
% Read file
while (~feof(fileID))
    temp = fgetl(fileID);
    if ~isempty(temp)
        if ~strcmp(temp(1),'%')
            s = s+1;
        end
    end
end

% Close file
fclose(fileID);

surf = zeros(s,18);

% Open file
fileID = fopen(loc,'r');

% Read file
j=1;
while (~feof(fileID))
    temp = fgetl(fileID);
    if ~isempty(temp)
        if ~strcmp(temp(1),'%')
            surf(j,:) = str2num(temp);
            j=j+1;
        end
    end
end

% Close file
fclose(fileID);

% Plot spacecraft in body frame
figure
hold on
ft = 16;
color = [0.7 0.7 0.7];
for i = 1:s
    fill3(surf(i,[4 7 10]),surf(i,[5 8 11]),surf(i,[6 9 12]),color)
end
set(gca,'Fontsize',ft)
xlabel('x-axis (m)','Fontsize',ft);
ylabel('y-axis (m)','Fontsize',ft);
zlabel('z-axis (m)','Fontsize',ft);
grid on

for i = 1:s
    quiver3(mean(surf(i,[4 7 10])),mean(surf(i,[5 8 11])),mean(surf(i,[6 9 12])),surf(i,1),surf(i,2),surf(i,3),'color','b');
end

end