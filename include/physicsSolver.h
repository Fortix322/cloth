#pragma once

#include <list>
#include <functional>

#include "glm.hpp"

struct VerletObject
{
	glm::fvec3 CurrentPosition;
	glm::fvec3 PreviousPosition;
	glm::fvec3 Acceleration;

	bool IsStatic = false;

	void SetPosition(glm::fvec3 pos)
	{
		CurrentPosition = pos;
		PreviousPosition = pos;
		Acceleration = {};
	}

	void UpdatePosition(float dt)
	{
		if (IsStatic) return;

		glm::fvec3 velocity = CurrentPosition - PreviousPosition;

		PreviousPosition = CurrentPosition;

		CurrentPosition = CurrentPosition + velocity + Acceleration * dt * dt;

		Acceleration = {};
	}

	void Accelerate(glm::fvec3 acc)
	{
		Acceleration += acc;
	}
};

struct Link
{
	VerletObject* lObject;
	VerletObject* rObject;
	float TargetDistance;

	void Apply()
	{
		VerletObject& lObj = *lObject;
		VerletObject& rObj = *rObject;
		glm::fvec3 axis = lObj.CurrentPosition - rObj.CurrentPosition;
		float distance = glm::length(axis);
		glm::fvec3 n = axis / distance;
		float delta = TargetDistance - distance;
		if (!lObj.IsStatic) lObj.CurrentPosition += 0.5f * delta * n;
		if (!rObj.IsStatic) rObj.CurrentPosition -= 0.5f * delta * n;
	}
};

struct PhysicsSolver
{
	void Update(VerletObject* objects, unsigned int count, float dt)
	{
		ApplyGravity(objects, count);
		UpdatePositions(objects, count, dt);
	}

	glm::fvec3 Gravity = { 0.0f, -9.0f, 0.0f };

private:

	void UpdatePositions(VerletObject* objects, unsigned int count, float dt)
	{
		for (size_t i = 0; i < count; i++)
		{
			objects[i].UpdatePosition(dt);
		}
	}

	void ApplyGravity(VerletObject* objects, unsigned int count)
	{
		for (size_t i = 0; i < count; i++)
		{
			objects[i].Accelerate(Gravity);
		}
	}
};