print("asdafsd")

local obj1 = GameObjectManager:GetObject("Object1");
print(obj1:GetName());

AudioManager:LoadAudio("test","../Resource/Audios/test.mp3");

AudioManager:PlayAudio("test");
AudioManager:PauseAudio("test");
AudioManager:SetAudioVolume("test",2);
AudioManager:ResumeAudio("test");