local xinit = 50
local yinit = 50

function retangulo (x,y,w,h)
local originalx, originaly, rx, ry, rw, rh =
x, y, x, y, w, h

local naimagem  =   function (mx, my, x, y)
                  return (mx>x) and (mx<x+w) and (my>y) and (my<y+h)
                end 
return {
  draw =  function ()
            love.graphics.rectangle("line", rx, ry, rw, rh)
          end,   
  keypressed =  function (key)
                  local mx, my = love.mouse.getPosition()
                    if key == 'b' and naimagem (mx,my, rx, ry) then
                        ry = yinit
                        rx = xinit
                    elseif key == 'down' and naimagem(mx, my, rx, ry)  then
                        ry = ry + 10
                    elseif key == 'right' and naimagem(mx, my, rx, ry)  then
                        rx = rx + 10
                    end
                end    
}
end
function love.load()
  ret = retangulo(xinit,yinit+150,200,300)
  ret2 = retangulo(500,40,60,70)
  retangulos = {retangulo(100,70,60,70),retangulo(200,20,60,70),retangulo(300,60,60,70)}
end

function love.keypressed(key)
  ret.keypressed(key)
  ret2.keypressed(key)
  
  for i = 1, #retangulos do 
    retangulos[i].keypressed(key)
  end
end

function love.update (dt) 
  if love.keyboard.isDown("down") then
    ret.keypressed("down");
    ret2.keypressed("down");
    for i = 1, #retangulos do 
      retangulos[i].keypressed("down")
    end
  elseif love.keyboard.isDown("right") then
    ret.keypressed("right");
    ret2.keypressed("right");
    for i = 1, #retangulos do 
      retangulos[i].keypressed("right");
    end
  end
end

function love.draw ()
  ret.draw("line", x, y, w, h)
  ret2.draw("line", x, y, w, h)
  for i = 1, #retangulos do 
    retangulos[i].draw("line", x, y, w, h)
  end
end
