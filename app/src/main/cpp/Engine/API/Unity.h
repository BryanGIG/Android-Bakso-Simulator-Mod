#include <android/log.h>
#include <cstdlib>
#include <uchar.h>
#include <stdio.h>
#include <string>
#include <inttypes.h>
#include <codecvt>
#include <locale>
#include <dlfcn.h>
#include "API.h"
#include "Rect.hpp"
#include "Color.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Matrix4x4.hpp"
#include "Quaternion.hpp"

static inline float NormalizeAngle(float angle) {
    while (angle > 360)
        angle -= 360;
    while (angle < 0)
        angle += 360;
    return angle;
}

static inline Vector3 NormalizeAngles(Vector3 angles) {
    angles.X = NormalizeAngle(angles.X);
    angles.Y = NormalizeAngle(angles.Y);
    angles.Z = NormalizeAngle(angles.Z);
    return angles;
}

static inline Vector3 ToEulerRad(Quaternion q1) {
    float Rad2Deg = 360.0 / (M_PI * 2.0);

    float sqw = q1.W * q1.W;
    float sqx = q1.X * q1.X;
    float sqy = q1.Y * q1.Y;
    float sqz = q1.Z * q1.Z;
    float unit = sqx + sqy + sqz + sqw;
    float test = q1.X * q1.W - q1.Y * q1.Z;
    Vector3 v;

    if (test > 0.4995 * unit) {
        v.Y = 2.0 * atan2f(q1.Y, q1.X);
        v.X = M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Deg);
    }
    if (test < -0.4995 * unit) {
        v.Y = -2.0 * atan2f(q1.Y, q1.X);
        v.X = -M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Deg);
    }
    Quaternion q(q1.W, q1.Z, q1.X, q1.Y);
    v.Y = atan2f(2.0 * q.X * q.W + 2.0 * q.Y * q.Z, 1 - 2.0 * (q.Z * q.Z + q.W * q.W)); // yaw
    v.X = asinf(2.0 * (q.X * q.Z - q.W * q.Y)); // pitch
    v.Z = atan2f(2.0 * q.X * q.Y + 2.0 * q.Z * q.W, 1 - 2.0 * (q.Y * q.Y + q.Z * q.Z)); // roll
    return NormalizeAngles(v * Rad2Deg);
}

static inline Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc) {
    return Quaternion::LookRotation((targetLocation + Vector3(0, y_bias, 0)) - myLoc,
                                    Vector3(0, 1, 0));
}

template<typename T>
struct monoArray {
    void *klass;
    void *monitor;
    void *bounds;
    int max_length;
    void *vector[1];

    int getLength() {
        return max_length;
    }

    T getPointer() {
        return (T) vector;
    }
};

template<typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems() {
        return items->getPointer();
    }

    auto operator[](int dex) {
        return items->getPointer()[dex];
    }

    int getSize() {
        return size;
    }

    int getVersion() {
        return version;
    }
};

template<typename K, typename V>
struct monoDictionary {
    void *unk0;
    void *unk1;
    monoArray<int **> *table;
    monoArray<void **> *linkSlots;
    monoArray<K> *keys;
    monoArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K getKeys() {
        return keys->getPointer();
    }

    V getValues() {
        return values->getPointer();
    }

    int getNumKeys() {
        return keys->getLength();
    }

    int getNumValues() {
        return values->getLength();
    }

    int getSize() {
        return size;
    }
};

union intfloat {
    int i;
    float f;
};

/*
Get the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
*/
static float GetObscuredFloatValue(uint64_t location){
    int cryptoKey = *(int *)location;
    int obfuscatedValue = *(int *)(location + 0x4);

    /* use this intfloat to set the integer representation of our parameter value, which will also set the float value */
    intfloat IF;
    IF.i = obfuscatedValue ^ cryptoKey;

    return IF.f;
}

/*
Set the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
	- value: the value we're setting the ObscuredFloat to
*/
static void SetObscuredFloatValue(uint64_t location, float value){
    int cryptoKey = *(int *)location;

    /* use this intfloat to get the integer representation of our parameter value */
    intfloat IF;
    IF.f = value;

    /* use this intfloat to generate our hacked ObscuredFloat */
    intfloat IF2;
    IF2.i = IF.i ^ cryptoKey;

    *(float *)(location + 0x4) = IF2.f;
}

static int GetObscuredInt(uint64_t location){
    int cryptoKey = *(int *)location;
    int encryptedValue = *(int *)(location + 0x4);
    int realValue = encryptedValue ^ cryptoKey;
    return realValue;
}

static void SetObscuredInt(uint64_t location, int value){
    int cryptoKey = *(int *)location;
    int encryptedValue = value ^ cryptoKey;
    *(int *)(location + 0x4) = encryptedValue;
}