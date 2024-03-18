#include "../Includes.hpp"

void (*CheatDetected)(GameManager *);

void new_CheatDetected(GameManager *) {}

uintptr_t hardLimitGachaKey;
uintptr_t GachaKey;
uintptr_t GemAmount;
uintptr_t _money;
uintptr_t specialToken;
uintptr_t gold;

void setCheat(GameManager *ins) {
    Il2Cpp::SetField<int>(ins, hardLimitGachaKey, INT_MAX);
    Il2Cpp::SetField<int>(ins, GachaKey, 999999999);
    Il2Cpp::SetField<int>(ins, GemAmount, 999999999);
    SetObscuredFloatValue((uintptr_t) ins + _money, 999999999.f);
    SetObscuredInt((uintptr_t) ins + specialToken, 999999999);
    SetObscuredInt((uintptr_t) ins + gold, 999999999);
}

void (*Update)(GameManager *);

void new_Update(GameManager *ins) {
    setCheat(ins);
    Update(ins);
}

void GameManager::Initialize() {
    setPointer(CheatDetected, "CheatDetected", 0);
    setPointer(Update, "Update", 0);
    setFieldOffset(hardLimitGachaKey, "hardLimitGachaKey");
    setFieldOffset(GachaKey, "GachaKey");
    setFieldOffset(GemAmount, "GemAmount");
    setFieldOffset(_money, "_money");
    setFieldOffset(specialToken, "specialToken");
    setFieldOffset(gold, "gold");
}

void GameManager::Hook() {
    setHook(Update, "Update");
    setHook(CheatDetected, "CheatDetected");
}

const char *GameManager::getAssembly() {
    return "Assembly-CSharp.dll";
}

const char *GameManager::getNamespace() {
    return "";
}

const char *GameManager::getClass() {
    return "GameManager";
}