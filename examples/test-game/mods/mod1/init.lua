print("Mod 1 Loaded!")

RegisterFunction(beforeGameUpdate, function ()
  print("(Mod 1): Before game update!")
end)

RegisterFunction(afterGameUpdate, function ()
  print("(Mod 1): After game update!")
end)
