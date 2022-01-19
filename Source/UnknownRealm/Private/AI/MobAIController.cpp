#include "AI/MobAIController.h"
#include "GameTeam.h"
#include "AI/AIChar.h"
#include "BehaviorTree/BehaviorTree.h"

ETeamAttitude::Type AMobAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    if (const APawn* OtherPawn = Cast<APawn>(&Other))
    {
        if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
        {
            //Create an alliance with Team with ID 10 and set all the other teams as Hostiles:
            const FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
            if (OtherTeamID == EGameTeam::Player)
            {
                return ETeamAttitude::Hostile;
            }
        }
    }

    return ETeamAttitude::Neutral;
}

void AMobAIController::BeginPlay()
{
    Super::BeginPlay();
	
    SetGenericTeamId(FGenericTeamId(EGameTeam::Monster));
}

void AMobAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AAIChar* AIChar = Cast<AAIChar>(InPawn);
	if (HasAuthority())
    {
        BaseTree->BlackboardAsset = AIChar->GetBlackboardData();
        RunBehaviorTree(BaseTree);
		if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent))
		{
            BTComp->SetDynamicSubtree(CombatTag, AIChar->GetCombatTree());
		}
    }
}
