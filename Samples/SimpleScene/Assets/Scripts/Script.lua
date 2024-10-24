local ecs = require "ecs"

local function lifecycleManager(context)
    for timer, destroy, position, entity in ecs.each(context) do
        timer.time = timer.time + context.delta_time

        if timer.time > destroy.toDestroy then
            position.y = 10000.0
        end
    end
end

local function collisionDetector(it)
    for positionFirst, collisionFirst, entityFirst in ecs.each(it) do
        for positionSecond, collisionSecond, entitySecond in ecs.each(it) do
            if entityFirst ~= entitySecond then
                local dx = positionFirst.x - positionSecond.x
                local dy = positionFirst.y - positionSecond.y
                local dz = positionFirst.z - positionSecond.z
                local distanceSq = dx * dx + dy * dy + dz * dz

                if distanceSq < 4.0 then
                    collisionFirst.isCollided = true
                    collisionSecond.isCollided = true
                end
            end
        end
    end
end

local function collisionResponder(it)
    for position, velocity, collision, entity in ecs.each(it) do
        if collision.isCollided then
            velocity.x = -velocity.x
            velocity.y = -velocity.y
            velocity.z = -velocity.z
            position.x = position.x + velocity.x
            collision.isCollided = false
        end
    end
end

ecs.system(lifecycleManager, "LifecycleManager", ecs.OnUpdate, "Timer, ToDestroy, Position")
ecs.system(collisionDetector, "CollisionDetector", ecs.OnUpdate, "Position, Collision")
ecs.system(collisionResponder, "CollisionResponder", ecs.OnUpdate, "Position, Velocity, Collision")
