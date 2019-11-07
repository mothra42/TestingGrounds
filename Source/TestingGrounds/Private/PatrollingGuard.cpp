// Fill out your copyright notice in the Description page of Project Settings.


#include "TestingGrounds\Public\PatrollingGuard.h"

TArray<AActor*> APatrollingGuard::GetPatrolPoints()
{
	return PatrolPointsCPP;
}