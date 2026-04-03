import("bootstrap")

local world = {}

local function cwd()
  local info = debug.getinfo(1, "S")
  if info and info.source then
    local path = info.source:gsub("^[@=]", "")
    return path:match("^(.*[\\/])")
  end

  return nil
end

local function sort_systems(systems)
  local system_sort = {}
  for _, system in pairs(systems) do
    table.insert(system_sort, system)
  end

  table.sort(system_sort, function(a, b)
    if a.priority and b.priority then
      return a.priority < b.priority
    elseif a.priority then
      return true
    else
      return false
    end
  end)

  return system_sort
end

function world.ensure()
  if not WORLD then
    ECS.utils.loadNamespace(cwd() .. "components")

    local Systems = {}
    ECS.utils.loadNamespace(cwd() .. "systems", Systems)

    WORLD = ECS.world()

    for _, system in ipairs(sort_systems(Systems)) do
      WORLD:addSystem(system)
    end
  end

  if not CAMERA then
    CAMERA = ECS.entity(WORLD)
  end
  CAMERA
      :ensure("position", 0, 0)
      :ensure("velocity", 0, 0)
      :give("speed", 2.0)
      :ensure("follow")

  if not PLAYER then
    PLAYER = ECS.entity(WORLD)
  end
  PLAYER
      :ensure("animator", import("animators/player"):Init())
      :ensure("position", 20, 20)
      :ensure("velocity", 0, 0)
      :ensure("local_player")
      :give("speed", 4.0)
      :give("box_collider", Vector2.new(40, 55), Vector2.new(30, 45))
      :give("controllable")
      :give("player_input", {
        up = "KEY_W",
        down = "KEY_S",
        left = "KEY_A",
        right = "KEY_D",
      })
  CAMERA.follow.target = PLAYER

  if not BOX then
    BOX = ECS.entity(WORLD)
  end
  BOX
      :ensure("position", 100, 200)
      :give("box_draw", 50, 50, Color.RED)
      :give("box_collider", Vector2.new(50, 50), Vector2.zero)

  return WORLD, CAMERA, PLAYER, BOX
end

return world