function a=f(x)
    i = 0
    while i < 125
        x = cos(x)
        disp(x)
        i = i + 1
    end
    a = x
    
endfunction

f(%pi)
