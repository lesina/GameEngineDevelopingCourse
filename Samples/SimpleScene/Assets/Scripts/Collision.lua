local ecs = require "ecs"

local COLLISION_DISTANCE_SQ = 25.0

local function detectCollisions(context)
    local entities = {}
    for position, obstacle, entity in ecs.each(context) do
        table.insert(entities, {position = position, obstacle = obstacle, entity = entity})
    end

    for i = 1, #entities do
        for j = i + 1, #entities do
            local a = entities[i]
            local b = entities[j]
            local dx = a.position.x - b.position.x
            local distanceSq = dx * dx

            if distanceSq < COLLISION_DISTANCE_SQ then
                a.obstacle.hasCollided = true
                b.obstacle.hasCollided = true
            else
                a.obstacle.hasCollided = false
                b.obstacle.hasCollided = false
            end
        end
    end
end

local function respondToCollisions(context)
    for velocity, obstacle, entity in ecs.each(context) do
        if obstacle.hasCollided then
            velocity.x = -velocity.x
        end
    end
end

ecs.system(detectCollisions, "CollisionDetector", ecs.OnUpdate, "Position, Obstacle")
ecs.system(respondToCollisions, "CollisionResponder", ecs.OnUpdate, "Velocity, Obstacle")