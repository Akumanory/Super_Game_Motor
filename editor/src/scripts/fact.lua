function fact (n)
    if n == 0 then
        return 1
    else
        return n * fact(n-1)
    end
end

function dofact()
    for i=0,3 do
        print(fact(3))
    end
end

return dofact
