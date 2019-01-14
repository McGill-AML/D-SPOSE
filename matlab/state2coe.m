%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FUNCTION NAME:        state2coe
%
% DESCRIPTION:          This function will convert potition and velocity to
%                       orbital elements
%
% AUTHOR:               Luc Sagnieres
% DATE:                 August 10, 2015
% VERSION:              1
%
% INPUT:                - R: 3x1 position vector (m)
%                       - V: 3x1 velocity vector (m)
%
% OUTPUT:               - a: semi-major axis (m)
%                       - i: inclination (rad)
%                       - e: eccentricity
%                       - w: argument of perigee (rad)
%                       - omega: right ascension of the ascending node
%                         (rad)
%                       - theta: true anomaly (rad)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [a,i,e,w,omega,theta] = state2coe(R,V)

eps = 1e-10;

% Gravitational parameter (m^3 s^2)
mu = 3.986004418*10^14;

r = norm(R);

v = norm(V);

vr = dot(R,V)/r;

%Angular momentum (m^2 s^-1)

H = cross(R,V);
h = norm(H);

% Inclination
i = acos(H(3)/h);


z = [0;0;1];
N = cross(z,H);
no = norm(N);
    
% Right ascension of the ascending node (rad)
    if (no ~= 0)
        omega = acos(N(1)/no);
        if (N(2) < 0)
            omega = 2*pi-omega;
        end
    else
        omega = 0;
    end

% Eccentricity

E = 1/mu*((v^2-mu/r).*R-r*vr.*V);
e = norm(E);

% Semi-major axis
a = h*h/mu/(1-e*e);


% Argument of Perigee (rad)
    if (no ~= 0)
        if (e > eps)
            w = acos(dot(N,E)/no/e);
            if (E(3) <0)
                w = 2*pi-w;
            end
        else
            w = 0;
        end
    else
        w = 0;
    end
    
% True anomaly (rad)

    if (e > eps) 
        theta = acos(dot(E,R)/e/r);
        if (vr < 0)
            theta = 2*pi - theta;
        end
    else
        cp = cross(N,R);
        if (cp(3) >= 0)
            theta = acos(dot(N,R)/no/r);
        else
            theta = 2*pi - acos(dot(N,R)/no/r);
        end
    end
    
end

