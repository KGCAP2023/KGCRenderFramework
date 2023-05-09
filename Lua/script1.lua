local obj = GameObjectManager:GetObject("gooood");
local trans = obj:GetTransform();

function Start()

    print(obj:GetName());

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

end

