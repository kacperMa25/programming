clc; clear all; close all;
 
function x = pochodnaPierwsza(f, x, h)
    x = (f(x + h) - f(x - h)) / (2 * h);
end


function [xm, i] = bisekcja(f, a, b, e)
    xm = (a + b) / 2;
    x1 = a;
    x4 = b;
    i = 1;

    while x4 - x1 > e && abs(pochodnaPierwsza(f, xm, 0.1)) > e
        L = x4 - x1;
        x2 = x1 + 0.25 * L;
        x3 = x4 - 0.25 * L;
        
        if f(x1) > f(x4)
            if f(x2) > f(x4) 
                x1 = xm;
                xm = x3;
                x4 = x4;
            else
                x1 = x2;
                xm = xm;
                x4 = x3;
            end
        else
            if f(x1) < f(x3)
                x1 = x1;
                x4 = xm;
                xm = x2;
            else
                x1 = x2;
                xm = xm;
                x4 = x3;
            end
        end
        i = i + 1;
    end
end

function [xm, i] = zlotyPodzial(f, a, b, e)
   phi = (sqrt(5) - 1) / 2;
   x1 = a;
   x4 = b;
   L = b - a;

   xm = b - phi * L;

   i = 1;

   while x4 - x1 > e && abs(pochodnaPierwsza(f, xm, 0.1)) > e
       L = x4 - x1;
       xm = x4 - phi * L;
       x3 = x1 + phi * L;

       if f(x1) > f(x4)
            x1 = xm;
            x4 = x4;
       else
           x1 = x1;
           x4 = x3;
       end

       i = i + 1;
   end
end



wybor = 2;

switch wybor
    case 1
        f = @(x) x.^2 - x;
        a = 0.25;
        b = 0.75;

    case 2
        f = @(x) 1/3 * x.^2 - 13/7 * x + 11;
        a = -10;
        b = 10;
    case 3
        f = @(x) x.^4 - 12 * x.^3 + x + 4;
        a = -2;
        b = 2;
    otherwise
        f = @(x) -x.^3 + 3 * x.^2 - 3 * x;
        a = -2;
        b = -1;
end


eps = 1e-5;

[x1, n1] = bisekcja(f, a, b, eps)
[x2, n2] = zlotyPodzial(f, a, b, eps)


t = a:0.01:b;
plot(t, f(t))
hold on;
plot(x1, f(x1), 'ro')
plot(x2, f(x2), 'rx')
