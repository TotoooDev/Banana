Banana = require "Banana"

function OnStart()
    Banana.LogInfo("Starting!")
end

function OnUpdate(timestep)
    transform = Banana.GetTransform()
    translation = transform:GetTranslation()
    Banana.LogInfo(translation)
end
