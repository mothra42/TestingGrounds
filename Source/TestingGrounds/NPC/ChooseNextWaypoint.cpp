// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "BehaviorTree/BTNode.h"
#include "GameplayTasksComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRouteComponent.h"

//TODO protect pointers
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Patrol Points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();

	auto PatrolRouteComponent = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!ensure(PatrolRouteComponent)) { return EBTNodeResult::Failed; }
	
	auto PatrolPoints = PatrolRouteComponent->GetPatrolPoints();
	//Warn about empty patrol route;
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
	}

	// Set next Waypoint
	auto BlackBoardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackBoardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackBoardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);


	// Cycle Index
	int32 NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackBoardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}