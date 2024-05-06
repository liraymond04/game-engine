local walk_anim = Anim.new({1}, 3, 1, 10)

print("walk_anim")

walk_anim.frame_functions[1] = function ()
    print("frame 1")
    walk_anim:NextFrame()
end

walk_anim.frame_functions[2] = function ()
    print("frame 2")
    walk_anim:NextFrame()
end

walk_anim.frame_functions[3] = function ()
    print("frame 3")
    walk_anim:NextFrame()
end

return walk_anim
