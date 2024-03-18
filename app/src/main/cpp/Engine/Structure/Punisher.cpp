#include "../Includes.hpp"


float maxNormalMoney;

float maxNormalStockAmount;

float maxNormalRobotDrinkAmount;

void (*PunisherStart)(Punisher *);

void new_PunisherStart(Punisher *ins) {
    LOGI(__FUNCTION__);
    Il2Cpp::SetField<float>(ins, maxNormalMoney, FLT_MAX);
    Il2Cpp::SetField<float>(ins, maxNormalStockAmount, FLT_MAX);
    Il2Cpp::SetField<float>(ins, maxNormalRobotDrinkAmount, FLT_MAX);
    return PunisherStart(ins);
}

void Punisher::Initialize() {
    setFieldOffset(maxNormalMoney, "maxNormalMoney");
    setFieldOffset(maxNormalStockAmount, "maxNormalStockAmount");
    setFieldOffset(maxNormalRobotDrinkAmount, "maxNormalRobotDrinkAmount");
    setPointer(PunisherStart, "Start", 0);
}

void Punisher::Hook() {
    setHook(PunisherStart, "Start");
}

const char *Punisher::getAssembly() {
    return "Assembly-CSharp.dll";
}

const char *Punisher::getNamespace() {
    return "";
}

const char *Punisher::getClass() {
    return "Punisher";
}
