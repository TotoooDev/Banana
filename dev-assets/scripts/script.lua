Banana = require "Banana"

function OnStart()
    Banana.LogInfo("Starting!")
    Banana.LogInfo(Entity)
end

function OnUpdate(timestep)
    transform = Banana.GetTransform()
    translation = transform:GetTranslation()
    translation:SetX(5.0)
    -- Banana.LogInfo(translation)
end
