local function newblip (vel)
  local x, y = 0, 0
  local width, height = love.graphics.getDimensions( )
  local novoBlip
  --Funções de New Blip:
  local function wait(segundos, blip)
    --colocar blip ativo
      blip.ativo = false
      blip.tempo = segundos
      coroutine.yield()
  end
  
  local up = coroutine.create(function()
    while true do
      x = x+ 10
      if x > width then
        -- volta para a esquerda da janela
        x = 0
      end
      --coroutine.yield ()
      wait((vel/10) ,novoBlip)
    end
  end)

  local function checkActive(dt)
      if novoBlip.ativo then
          coroutine.resume(up)
      else
        novoBlip.tempo = novoBlip.tempo - dt
        if novoBlip.tempo <= 0 then
          novoBlip.tempo = 0
          novoBlip.ativo = true
        end      
      end   
  end

  novoBlip = {
      update = checkActive,
      affected = function (pos)
        if pos>x and pos<x+10 then
        -- "pegou" o blip
          return true
        else
          return false
        end
      end,
      draw = function ()
        love.graphics.rectangle("line", x, y, 10, 10)
      end,
      ativo = true,
      tempo = 0
    }
    return novoBlip
end

local function newplayer ()
  local x, y = 0, 200
  local width, height = love.graphics.getDimensions( )
  return {
  try = function ()
    return x
  end,
  update = function (dt)
    x = x + 0.5
    if x > width then
      x = 0
    end
  end,
  draw = function ()
    love.graphics.rectangle("line", x, y, 30, 10)
  end
  }
end

function love.keypressed(key)
  if key == 'a' then
    pos = player.try()
    for i in ipairs(listabls) do
      local hit = listabls[i].affected(pos)
      if hit then
        table.remove(listabls, i) -- esse blip "morre" 
        return -- assumo que apenas um blip morre
      end
    end
  end
end

function love.load()
  player =  newplayer()
  listabls = {}
  for i = 1, 5 do
    listabls[i] = newblip(i)
  end
end

function love.draw()
  player.draw()
  for i = 1,#listabls do
    listabls[i].draw()
  end
end

function love.update(dt)
  player.update(dt)
  for i = 1,#listabls do
    listabls[i].update(dt)
  end
end