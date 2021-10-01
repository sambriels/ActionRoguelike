#include "SBlackholeProjectile.h"

ASBlackholeProjectile::ASBlackholeProjectile() {
  ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
  ForceComp->SetupAttachment(SphereComp);
}
