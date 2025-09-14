# Python to C++ Script Translation for GemRB

This directory contains the implementation for translating GemRB's Python GUI scripts to native C++.

## Overview

GemRB currently uses Python for GUI scripting through the GUIScript plugin. This provides flexibility but introduces Python as a dependency and performance overhead. The goal is to translate these scripts to native C++ while maintaining the same functionality.

## Translation Approach

### 1. Constant Definitions (Completed)

Many Python files just contain constant definitions that are already synchronized with C++ headers:

**Python (ie_action.py):**
```python
ACT_STEALTH=0
ACT_THIEVING=1
ACT_CAST=2
```

**C++ (ActionConstants.h):**
```cpp
namespace ActionConstants {
    const int ACT_STEALTH = 0;
    const int ACT_THIEVING = 1;
    const int ACT_CAST = 2;
}
```

**Already Synchronized Files:**
- `ie_stats.py` ↔ `ie_stats.h`
- `ie_restype.py` ↔ `SClassID.h`
- `GUIDefines.py` ↔ `Button.h`
- `ie_action.py` ↔ `PCStatStruct.h`

### 2. Simple Logic Scripts

**Python (Clock.py):**
```python
def UpdateClock():
    Hours = (GemRB.GetGameTime() % 7200) // 300
    Clock.SetBAM("CDIAL", 0, int((Hours + 12) % 24))
```

**C++ (ClockScript.cpp):**
```cpp
void UpdateClock() {
    unsigned long gameTime = CppAPI::GetGameTime();
    int hours = (gameTime % 7200) / 300;
    clock->SetPicture("CDIAL", 0, (hours + 12) % 24);
}
```

### 3. Complex GUI Scripts

For larger scripts like `ActionsWindow.py` (1643 lines), the approach is:

1. Create C++ class equivalent to Python module
2. Translate functions to methods
3. Use C++ API wrapper for GemRB calls
4. Maintain same function signatures for compatibility

## Files Structure

- `CppScript.h/cpp` - Main script engine implementing ScriptEngine interface
- `CppScriptAPI.h/cpp` - C++ wrapper providing GemRB Python API equivalents
- `ClockScript.h/cpp` - Translation of Clock.py functionality
- `ActionConstants.h` - Translation of ie_action.py constants
- `ModalConstants.h` - Translation of ie_modal.py constants
- `PythonToCppDemo.cpp` - Examples showing translation patterns

## Benefits of C++ Translation

1. **Performance**: Native C++ execution vs Python interpretation
2. **Dependencies**: Removes Python runtime dependency
3. **Memory**: Lower memory footprint
4. **Integration**: Better integration with C++ codebase
5. **Type Safety**: Compile-time error checking

## Implementation Status

- [x] **Plugin Infrastructure**: CppScript plugin framework
- [x] **Constant Translations**: ie_action.py, ie_modal.py examples
- [x] **API Wrapper**: Basic GemRB API equivalents in C++
- [x] **Clock Script**: Basic Clock.py translation
- [ ] **Build Integration**: Complete compilation and testing
- [ ] **Complex Scripts**: ActionsWindow.py, GUICommon.py, etc.
- [ ] **Configuration**: Runtime selection between Python/C++ engines

## Usage

The C++ script engine will be a drop-in replacement for the Python engine:

```cpp
// Engine will load C++ scripts instead of Python
core->LoadScript("Clock");  // Loads ClockScript.cpp functions
core->RunFunction("Clock", "UpdateClock", params);
```

## Testing

1. Compile the CppScript plugin
2. Configure GemRB to use IE_CPP_SCRIPT_CLASS_ID instead of IE_GUI_SCRIPT_CLASS_ID  
3. Test Clock functionality as proof of concept
4. Gradually enable more translated scripts

## Next Steps

1. Fix compilation issues and complete plugin build
2. Test Clock script functionality
3. Add configuration option to choose Python vs C++ engine
4. Translate more complex scripts (GUICommon.py, ActionsWindow.py)
5. Performance benchmarking vs Python version