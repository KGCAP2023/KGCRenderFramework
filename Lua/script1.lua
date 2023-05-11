local obj = GameObjectManager:GetObject("gooood");
local obj2 = Script:GetThisObject();

local trans = obj2:GetTransform();

function Start()

    print(obj:GetName());
    print(obj2:GetName());

end

function Update()

    if (KeyInput_W() == true) then  
        trans:MoveUp();
    end

    if (KeyInput_A() == true) then  
        trans:MoveLeft();
    end

    if (KeyInput_S() == true) then  
        trans:MoveDown();
    end

    if (KeyInput_D() == true) then  
        trans:MoveRight();
    end

    -- Camera:TraceObject(obj2); --3D오브젝트만 가능

end

