local led1 = 3
local led2 = 6
local sw1 = 1
local sw2 = 2

gpio.mode(led1, gpio.OUTPUT)
gpio.mode(led2, gpio.OUTPUT)

gpio.write(led1, gpio.LOW);
gpio.write(led2, gpio.LOW);

gpio.mode(sw1,gpio.INT,gpio.PULLUP)
gpio.mode(sw2,gpio.INT,gpio.PULLUP)

local tempoaceso = 200000
local seqrodada = {}
local tamseq = 5
local numeroSeq = 0
local function contabiliza1(level,timestamp)
    --Contabiliza aperto 1
    if (timestamp > tempo + 250000) then
        tempo = timestamp
        print(1)
        if (seqrodada[numeroSeq + 1] ~= sw1) then
             print("Errou")
             gpio.trig(sw1)
             gpio.trig(sw2)
             gpio.write(led1, gpio.HIGH)
        elseif (numeroSeq == 4) then
            print("Ganhou")
            gpio.write(led2, gpio.HIGH)
            gpio.trig(sw1)
            gpio.trig(sw2)
        else
            gpio.write(3, gpio.HIGH)
            tmr.delay(3*tempoaceso)
            gpio.write(3, gpio.LOW)
            tmr.delay(2*tempoaceso)
        end
        
    numeroSeq= numeroSeq + 1
    end
end

local function contabiliza2(level,timestamp)
     --Contabiliza aperto 1
    if (timestamp > tempo + 250000) then
        tempo = timestamp
        print(2)
        if (seqrodada[numeroSeq + 1] ~= sw2) then
             print("Errou")
             gpio.trig(sw1)
             gpio.trig(sw2)
             gpio.write(led1, gpio.HIGH)
        elseif (numeroSeq == 4) then
            print("Ganhou")
            gpio.write(led2, gpio.HIGH)
            gpio.trig(sw1)
            gpio.trig(sw2)
        else
            gpio.write(6, gpio.HIGH)
            tmr.delay(3*tempoaceso)
            gpio.write(6, gpio.LOW)
            tmr.delay(2*tempoaceso)
        end
     numeroSeq= numeroSeq + 1
    end
end

local function contabilizaJogadas()
    tempo = 0
    gpio.trig(sw1, "down", contabiliza1)
    gpio.trig(sw2, "down", contabiliza2)
end

local function geraseq (semente)
  print ("veja a sequencia:")
  tmr.delay(2*tempoaceso)
  print ("(" .. tamseq .. " itens)")
  math.randomseed(semente)
  for i = 1,tamseq do
    seqrodada[i] = math.floor(math.random(1.5,2.5))
    print(seqrodada[i])
    gpio.write(3*seqrodada[i], gpio.HIGH)
    tmr.delay(3*tempoaceso)
    gpio.write(3*seqrodada[i], gpio.LOW)
    tmr.delay(2*tempoaceso)
  end

end

local function cbchave1 (_,contador)
  -- corta tratamento de interrupções
  -- (passa a ignorar chave)
  gpio.trig(sw1)
  -- chama função que trata chave
  geraseq (contador)

  print("Aperte botoes")
  contabilizaJogadas()
end

gpio.trig(sw1, "down", cbchave1)
