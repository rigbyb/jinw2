print = Msg

local IN_ATTACK = (1 << 0)
local IN_JUMP	= (1 << 1)

local color_white = Color.new(255, 255, 255, 255)
local font = draw.CreateFont("Tahoma", 20)

function Think(usercmd)
    if usercmd.buttons & IN_JUMP == IN_JUMP then
        usercmd.buttons = usercmd.buttons | IN_ATTACK;
    end
end

function Paint()
    draw.Text("Welcome to jinw2 base", font, 100, 100, color_white)

    for _, ent in ipairs(ents.FindByClass("CTFPlayer")) do
        local pos = ent:GetAbsOrigin()

        if pos then
            local screen = pos:ToScreen()

            if screen.visible then
                draw.Text("Player", font, screen.x, screen.y, color_white)
            end
    end
    end
end