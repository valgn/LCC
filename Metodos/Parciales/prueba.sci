z = linspace(0, 25, 150);
x = z .* cos(z);
y = z .* sin(z);
scatter3(x, y, z, , "turquoise", "fill")
