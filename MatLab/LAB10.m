clc
close all
clear all

function [xb, nb] = bisekcja(f, a, b, tol, ftol)
    if f(a) * f(b) > 0
       error("Na przedziale nie ma pierwiastka")
    end
    xb = (a + b) / 2;
    nb = 1;
    prevxb = b;

    while abs(f(xb)) > ftol && abs(xb - prevxb) > tol
       if f(a) * f(xb) < 0
          b = xb;
       else
          a = xb;
       end
       prevxb = xb;
       xb = (a + b) / 2;
       nb = nb + 1;
    end
end

function x = pochodnaDruga(f, x, h)
  x = (f(x + h) - 2 * f(x) + f(x - h)) / h.^2;
end

function [xf, nf] = falsi(f, a, b, tol, ftol)
    if f(a) * f(b) > 0
        error("Na przedziale nie ma pierwiastka")
    end

    if f(a) * pochodnaDruga(f, a, tol) > 0
        xs = a;
        xf = b;
    else
        xs = b;
        xf = a;
    end
    nf = 1;
    prevxf = xf;
    xf = xs - f(xs) / (f(xf) - f(xs)) * (xf - xs);
    while abs(f(xf)) > ftol && abs(xf - prevxf) > tol
        xf = xs - f(xs) / (f(xf) - f(xs)) * (xf - xs);
        nf = nf + 1;
    end
     
end


wybor = 1;

switch wybor
    case 1
        f = @(x) cos(x);
        a = 0;
        b = 2;
    case 2
        f = @(x) 2.^(-x) + exp(x) + 2 .* cos(x) - 6;
        a = 1;
        b = 3;
    case 3
        f = @(x) (x + 2).^5;
        a = -3;
        b = 0;
    case 4
        f = @(x) x - sqrt(2);
        a = 0;
        b = 2;
end

tol = 10^(-5);
ftol = 10^(-4);

[xb, nb] = bisekcja(f, a, b, tol, ftol)

[xf, nf] = falsi(f, a, b, tol, ftol)

t = a:0.01:b;
plot(t, f(t))
hold on;
plot(xb, f(xb), 'ro')
plot(xf, f(xf), 'rx')
