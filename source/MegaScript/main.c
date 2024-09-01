//MegaScript!
//Created by Dynamagon https://www.youtube.com/@dynamagon
//I hope you enjoy! :D

//Information:
/*
    Update: The Objects Update!
    Version: 2.1.0 Alpha
    Date: Sunday August 20th, 2024, 8:21 PM
*/

//Update log:
/*
    -You can now create variables to hold objects! (Example: object a = root)
    -Fixed a bug that caused a segmentation fault when the master script is stopped due to timeout.
    -Optimized the memory usage of the memory section allocate and free functions.
    -Fixed file loading for Windows!
    -Removed the "length" member from the script struct as it is unnecessary.
    -Fixed typo in RemoveChild();
    -Fixed a bug with standard calls/keywords.
    -Added SetParent() to objects!
    -Added a new object/symbol initialization interface. InitializeEnvironmentSymbols(ScriptEnvironment*, SymbolInitializer*, unsigned int);
    -Added the ability for scripts to create objects!
    -It is no longer required to type "new" before structs, enums, or bools.
    -When an object is deleted, the MegaScript environment scans all scripts' stacks and symbol tables for the object's pointer. Then, they are all changed to 0 in order to prevent segmentation faults.
    -Bool containers now create a typedef so that the same settings can be used for multiple variables.
    -Added passthrough functions! You can now directly link a C/C++ function to megascript using the CFunctionType.
    -You can now create an environment that uses real memory addresses with CreateNonVirtualizedScriptEnvironment();
    -Fixed memory leaks with debris symbols.
    -String double quotes now push char arrays onto the stack instead of raw pointers!
*/

//Instructions:
/*
    This is meant to be used as a header file, but has the .c extension so that visual studio and other IDEs don't
    mark it as a C++ header file. (Rage)

    To start working with MegaScripts, first create a ScriptEnvironment like this:
        ScriptEnvironment *script_tasks = CreateScriptEnvironment();

    File types for MegaScript are the following:
        .mega = MegaScript Source
        .mlib = MegaScript Library
        .mcs = MegaScript Compiled Source (Will be used for obfuscation)

    Including:
    
        Make sure that the initialize.mega file as well as the Libraries folder are both in the working directory of your program so that they can be
        loaded into memory by MegaScript.

    Scopes:

        Scopes are what tell programming languages where variables can be accessed from. Without scopes, variables in if statements or while loops
        would be accessible everywhere, and making multiple definitions of the same variable would not be possible.

        Scopes are a bit different in MegaScript than in most languages. One bonus of the scope system in MegaScript is that scopes are spacial
        ranges. This allows for both block structured, and non structured programming like jumping directly to code addresses via a label. Scopes can
        also be found simply by memory address which adds lots of flexibility for the MegaScript language.

        Each child scope has its own id so that it can be uniquely identified in scenarios where there are two scopes starting at the same address,
        but the top level scope will always have -1 as its id since a struct is not required to represent the entire memory.

    Warnings:

        Variables and function names can be 256 characters long at max. This is because the symbol syntax uses one byte
        to notate the symbol length in order to save space.
*/

//Stats:
/*
    Max function parameters: 65535
    Max operators per statement: 65535
    Max symbol length: 256
    Max label length: 256
    Max string length: 18446744073709551615
    Max scopes: 18446744073709551614
    Max extended standard calls: 65536
    Max precedence groups: 255
    Max pointer dimensions: 255
    Max array dimensions: 255

    (Note: These are the max values that the corresponding attributes can be set to,
    regardless of user settings. This is due to the way the scripts are encoded.)
*/

//====code begin====

//standard libraries
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//os specific libraries
#ifdef _WIN32
#include <sys\timeb.h>
#include <windows.h>
#include <windowsx.h>
#elif linux
#include <sys/time.h>
#include <unistd.h>
#endif

//MegaScript includes
#include "C/enums.c"
#include "C/structs.c"
#include "C/settings.c"
#include "C/prototypes.c"
#include "C/messages.c"
#include "C/convenient.c"
#include "C/object.c"
#include "C/symbols.c"
#include "C/interface.c"
#include "C/scope.c"
#include "C/string.c"
#include "C/interrupts.c"
#include "C/standard.c"
#include "C/scripts.c"
#include "C/stack.c"
#include "C/files.c"
#include "C/memory.c"
#include "C/parser.c"
#include "C/ftoa.c"
#include "C/arithmetic.c"
#include "C/calls.c"
#include "C/execute.c"

//====code end====
