// Fill out your copyright notice in the Description page of Project Settings.
#include "ChooseNextWaypoint.h"
#include "TestingGrounds.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

    // Get the patrol route
    auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
    if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

    // Warn about empty patrol routes
    auto PatrolPoints = PatrolRoute->GetPatrolPoints();
    if (PatrolPoints.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
        return EBTNodeResult::Failed;
    }

    // Set next waypoint
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
    BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
    UE_LOG(LogTemp, Warning, TEXT("%i"), Index);

    // Cycle the index
    auto NextIndex = (Index + 1) % PatrolPoints.Num();
    BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

    return EBTNodeResult::Succeeded;
}