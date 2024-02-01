print("Core Mod loaded!")

RegisterFunction("HOOK_BEFORE_MAIN_MENU_INIT", function ()
  print("(Core Mod): Before main menu init!")
end)

RegisterFunction("HOOK_AFTER_SETTINGS_MENU_CLEANUP", function ()
  print("(Core Mod): After main menu cleanup!")
end)
