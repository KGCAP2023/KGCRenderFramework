local obj2 = Script:GetThisObject();

local trans = obj2:GetTransform();

function Start()

    print(obj2:GetName());
    obj2:LoadAnimPreset("man");

end

function Update()

    obj2:PlayAnim2D("idle",0);

    if (KeyInput_W() == true) then  
        trans:MoveUp();
    end

    if (KeyInput_A() == true) then  
        trans:MoveLeft();
        obj2:PlayAnim2D("run",1);
    end

    if (KeyInput_S() == true) then  
        trans:MoveDown();
    end

    if (KeyInput_D() == true) then  
        trans:MoveRight();
        obj2:PlayAnim2D("run",0);
    end

    
    -- Camera:TraceObject(obj2); --3D오브젝트만 가능

end

