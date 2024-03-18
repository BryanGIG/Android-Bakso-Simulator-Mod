## Android Bakso Simulator Mod

As it names!

### How To Use

1. Clone the repo
2. Open the project in Android Studio / Visual Studio Code
3. Change the package name in `app/build.gradle` and library name
   in `app/src/main/cpp/CMakeLists.txt`
4. Start coding
5. Build the project
6. Copy the generated .so from `app/build/outputs/native/{debug/release}/lib{bryangig}.so`
7. Load the library in your modded app
   </br>You can use many various methods to load the library, see [Injecting the library](#injecting-the-library)

### Injecting the library
- #### [Android-Ptrace-Injector - reveny](https://github.com/reveny/Android-Ptrace-Injector)
```bash
./Injector -p "com.target.package" -l "bryangig"
```

- #### [System.loadLibrary](https://docs.oracle.com/javase/8/docs/api/java/lang/System.html#loadLibrary-java.lang.String)
```java
const-string v0, "bryangig"

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

- #### [xDL - hexhacking](https://github.com/hexhacking/xDL)
```c
void* handle = xdl_open("bryangig", RTLD_NOW);
```

- #### Other methods
As long as you can load the library into the target process, you can use any method you want.

### To-Do
- [x] Polymorphism design pattern
- [x] Unity API (Vector2, Vector3, Quaternion, etc)
- [x] Doc: How to use
- [ ] Doc: What need to be changed
- [ ] Doc: What each file does
- [ ] Doc: How to add new functions
- [ ] Doc: How to add new classes

### Credit

- [Dobby](https://github.com/jmpews/Dobby) - Hooking library
- [KittyMemory](https://github.com/MJx0/KittyMemory) - Android, IOS runtime code patching library
