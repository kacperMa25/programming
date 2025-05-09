clc
close all
clear all

function [ x1 , x2 , dx1 , dx2 , it ] = sww (f , xpocz , ypocz , krok , eps , x1p , x1k , x2p , x2k )

    x1 = xpocz;
    x2 = ypocz;
    it = 0;
    
    wersor = [[1, 0];
        [0, 1];
        [-1, 0];
        [0, -1];
        [sqrt(2)/2, sqrt(2)/2];
        [-sqrt(2)/2, -sqrt(2)/2];
        [sqrt(2)/2, -sqrt(2)/2];
        [-sqrt(2)/2, sqrt(2)/2]];

    dx1 = [x1];
    dx2 = [x2];
   

    while true
        
        z = f(x1, x2);

        mat = [[x1, x2];
            [x1, x2];
            [x1, x2];
            [x1, x2];
            [x1, x2];
            [x1, x2];
            [x1, x2];
            [x1, x2];] + wersor .* krok;

        F = f(mat(:, 1), mat(:, 2));

        [M, n] = min(F);

        if M < z
            x1 = mat(n, 1);
            x2 = mat(n, 2);
            dx1 = [dx1, x1];
            dx2 = [dx2, x2];
            it = it + 1;

        else if krok > eps
            krok = krok * 0.5;
        else
            break;
        end
        end
    end

end

wybor = 3;

switch wybor
case 1
f = @(x1, x2) x1 .^2 + x2.^2;
x1p = -10;
x1k = 10;
x2p = -10;
x2k = 10;
xpocz = 8;
ypocz = -10;
krok = 1;
case 2
f = @(x1, x2) -cos(x1) .* cos(x2) .* exp(-((x1 - pi ).^2 + (x2 - pi).^2));
x1p = 2;
x1k = 4;
x2p = 2;
x2k = 4;
xpocz = 2;
ypocz = 2;
krok = 0.2;
otherwise
f = @(x1, x2) (1 - 8 .* x1 + 7 .* x1 .^2 - 7/3 .* x1 .^ 3 + 1/4 .* x1 .^ 4) .* (x2 .^2 .* exp(-x2));
x1p = 0;
x1k = 5;
x2p = 0;
x2k = 5;
xpocz = 5;
ypocz = 5;
krok = 0.2;
end


eps = 1e-4;
[ x1 , y2 , dx1 , dx2 , it ] = sww (f , xpocz , ypocz , krok , eps , x1p , x1k , x2p , x2k)


subplot (2 ,1 ,1)
% wykres funkcji 3 d na zadanym przedziale - meshgrid + mesh
[x , y ] = meshgrid (x1p:krok:x1k, x2p:krok:x2k) ;
mesh (x ,y , f (x , y ) ) ;
hold on
% zaznaczone minimum - plot3

subplot (2 ,1 ,2) ;
z = f (x , y ) ;
[c , h ]= contour (x ,y , z ) ;
clabel (c , h ) ;
hold on
% zaznaczenie i podpisanie punktu startowego
plot (xpocz, ypocz, 'r * ') ;
text ( xpocz, ypocz, ' START ') ;
% zaznaczenie i podpisanie punktu koncowego - minimum
plot ( x1, y2, 'r * ') ;
text ( x1, y2, ' STOP ') ;

% narysowanie sciezki - linie pomiedzy kolejnymi punktami
for i =1: length ( dx1 ) -1
    line ([dx1(i), dx1(i + 1)], [dx2(i), dx2(i + 1)] , 'Color', 'red' , 'Linewidth', 1) ;
    %contour (x ,y ,z ,[ f(dx1(i), dx2(i)), f(dx1(i) , dx2(i))]) ;
end


