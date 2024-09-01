int StandardCallPrintTest(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    printf("hi it worked\n");
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardPushNull(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, NullType);
}

int StandardEchoCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    return PrintCallDataHandler(script_tasks, subject_script, stack_symbol, MakeNewLine);
}

int StandardRoot(ScriptEnvironment *script_tasks, Script *subject_script) {
    uint64 root_number = (uint64) script_tasks->root;
    if(!root_number) return EnvironmentRootNotFoundError;
    return PushScriptStackData(subject_script, (byte*) &root_number, ObjectType);
}

int StandardMakeConst(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(stack_symbol.error) return stack_symbol.error;
    
    switch(stack_symbol.type) {
        case SymbolType:
            Symbol *current_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
            if(current_symbol) {
                current_symbol->settings |= IsConstant;
                //printf("I'm now a constant! :D\n");
                return PushScriptStackData(subject_script, stack_symbol.data, SymbolType);
            } else {
                return CouldNotFindTheSymbolError;
            }
        default:
            return AttemptedSymbolSettingsChangeOnNonSymbolError;
    }
}

int StandardMakeGlobal(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(stack_symbol.error) return stack_symbol.error;

    switch(stack_symbol.type) {
        case SymbolType:
            Symbol *current_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
            if(current_symbol) {
                current_symbol->created_at = 0;
                current_symbol->scope = -1;
                return PushScriptStackData(subject_script, stack_symbol.data, SymbolType);
            } else {
                return CouldNotFindTheSymbolError;
            }
        default:
            return AttemptedSymbolSettingsChangeOnNonSymbolError;
    }
}

int StandardDeleteCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(stack_symbol.error) return stack_symbol.error;

    switch(stack_symbol.type) {
        case SymbolType:
            uint64 symbol_id = *(uint64*) stack_symbol.data; //pointer marker
            int64 symbol_index = FindSymbolById(subject_script, symbol_id);
            if(symbol_index == -1) return CouldNotFindTheSymbolError;
            int error_code = DeleteSymbol(script_tasks, subject_script, symbol_index);
            if(error_code) return error_code;
            return PushScriptStackData(subject_script, 0, VoidType);
        default:
            return AttemptedSymbolSettingsChangeOnNonSymbolError;
    }
}

int StandardMemsetCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardMemcpyCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardInputCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardErrorCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardAllocCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    if(stack_data.error) return stack_data.error;
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    if(stack_data.error) return stack_data.error;
    uint64 range_address = AllocateScriptMemory(script_tasks, subject_script, CastToUnsignedInt64(stack_data));
    return PushScriptStackData(subject_script, (byte*) &range_address, PointerType); //pointer marker
}

int StandardFreeCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    if(stack_data.error) return stack_data.error;
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    if(stack_data.error) return stack_data.error;
    int return_value = FreeScriptMemory(script_tasks, subject_script, CastToUnsignedInt64(stack_data));
    return PushScriptStackData(subject_script, (byte*) &return_value, Int32Type); //pointer marker
}

int StandardRunCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardTypeofCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    if(stack_data.error) return stack_data.error;
    if(!stack_data.data) return StackEmptyError;
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    if(stack_data.error) return stack_data.error;
    byte data_length = (byte) stack_data.length;
    return PushScriptStackData(subject_script, &stack_data.type, DataType);
}

int StandardScriptCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardShellCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    FILE *file_ptr;
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(stack_symbol.error) return stack_symbol.error;
    if(!stack_symbol.data) return StackEmptyError;
    stack_symbol = ExpandStackData(script_tasks, subject_script, stack_symbol);
    if(stack_symbol.error) return stack_symbol.error;
    uint64 data_address = 0;

    switch(stack_symbol.type) {
        case PointerType:
            data_address = GetPointerStackData(stack_symbol);
            stack_symbol = GetStackDataFromMemory(script_tasks, subject_script, &data_address);
            switch(stack_symbol.type) {
                case StringType:
                    byte *string_ptr = GenerateValidString(script_tasks, subject_script, stack_symbol.data - script_tasks->memory, stack_symbol.length);
                    system(string_ptr);
                    break;
                default:
                    return InvalidArgumentError;
            }
            break;
        default:
            return InvalidArgumentError;
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardTimeCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    float64 time_value = GetCurrentOSTime();
    return PushScriptStackData(subject_script, (byte*) &time_value, DoubleType);
}

int StandardLoadLibCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return StandardLoadCall(script_tasks, subject_script, LibraryScriptType);
}

int StandardReserveSizeCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    uint64 size_value = script_tasks->memory_reserve;
    return PushScriptStackData(subject_script, (byte*) &size_value, UnsignedInt64Type);
}

int StandardRequireCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    int32 return_value = 1;
    StackData name_string = PopScriptStackData(subject_script);
    if(name_string.error) return name_string.error;
    byte *string_buffer = GetStringFromStackData(script_tasks, subject_script, name_string);
    if(!string_buffer) return UnableToCreateStringError;
    int64 script_index = FindScriptByName(script_tasks, string_buffer);
    if(script_index == -1) return ScriptNameNotFoundError;
    Script *selected_script = *(script_tasks->scripts + script_index);
    if(selected_script->type == StandardScriptType) return RequiredStandardScriptError;
    return_value = 0;
    for(int j = 0; j < selected_script->values.symbol_count; j++) {
        Symbol *current_symbol = selected_script->values.symbol_table + j;
        if(CreateSymbol(script_tasks, subject_script, *current_symbol) == -1) {
            ExecuteStandardWarnCallbacks(script_tasks, subject_script, "[Warning]: Failed to copy all the symbols from the library.");
            return_value = 1;
            break;
        }
    }
    free(string_buffer);
    return PushScriptStackData(subject_script, (byte*) &return_value, Int32Type);
}

int StandardTitleCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData title_string = PopScriptStackData(subject_script);
    if(title_string.error) return title_string.error;
    byte *string_buffer = GetStringFromStackData(script_tasks, subject_script, title_string);
    if(!string_buffer) return UnableToCreateStringError;
    uint64 length = strlen(string_buffer);
    if(subject_script->system.settings & NameWasMalloced) free(subject_script->values.name);
    subject_script->system.settings |= NameWasMalloced;
    subject_script->values.name = string_buffer;
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardPushCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardPopCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data_0 = PopScriptStackData(subject_script);
    if(stack_data_0.error) return stack_data_0.error;
    byte data_size = GetDataTypeLength(stack_data_0.type);
    byte *new_buffer = malloc(data_size);
    memcpy(new_buffer, stack_data_0.data, data_size);
    stack_data_0.data = new_buffer;
    int error_code = 0;
    if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, 0)) return error_code;
    StackData stack_data_1 = PopScriptStackData(subject_script);
    if(stack_data_1.error || stack_data_0.error) StackEmptyError;
    if(error_code = AssignData(script_tasks, subject_script, stack_data_0, stack_data_1)) return error_code;
    free(stack_data_0.data);
    return 0;
}

int StandardCreateCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData current_data = PopScriptStackData(subject_script);
    if(current_data.error) return current_data.error;
    char *stack_string = GetStringFromStackData(script_tasks, subject_script, current_data);
    if(!stack_string) return FailedToPopStringError;
    ObjectClassInformation *class_info = GetObjectClassInformationByName(script_tasks, stack_string);
    if(!class_info) return FailedToFindClassError;
    if(subject_script->rules.privilege_level < class_info->privilege_level) return PrivilegeLevelError;
    void *class_data = malloc(class_info->content_size);
    if(class_info->preset) memcpy(class_data, class_info->preset, class_info->content_size); else memset(class_data, 0, class_info->content_size);
    Object *new_object = CreateObject(stack_string, class_info->class, class_data);
    uint64 class_pointer = (uint64) new_object; //pointer marker
    return PushScriptStackData(subject_script, (byte*) &class_pointer, ObjectType);
}

#define standard_call_count 24
static StandardCall default_standard_calls[] = {
    {"input", 0, LowPrivilege, StandardInputCall},
    {"null", 0, RestrictedPrivilege, StandardPushNull},
    {"echo", 1, RestrictedPrivilege, StandardEchoCall},
    {"root", 0, LowPrivilege, StandardRoot},
    {"const", 1, LowPrivilege, StandardMakeConst},
    {"global", 1, SuperPrivilege, StandardMakeGlobal},
    {"delete", 1, SuperPrivilege, StandardDeleteCall},
    {"error", 1, LowPrivilege, StandardErrorCall},
    {"alloc", 1, MediumPrivilege, StandardAllocCall},
    {"free", 1, MediumPrivilege, StandardFreeCall},
    {"allocate", 1, MediumPrivilege, StandardAllocCall},
    {"typeof", 1, RestrictedPrivilege, StandardTypeofCall},
    {"run", 1, SuperPrivilege, StandardRunCall},
    {"script", 0, LowPrivilege, StandardScriptCall},
    {"shell", 1, SuperPrivilege, StandardShellCall},
    {"time", 0, LowPrivilege, StandardTimeCall},
    {"loadlib", 1, MasterPrivilege, StandardLoadLibCall},
    {"memory_reserve", 0, RestrictedPrivilege, StandardReserveSizeCall},
    {"require", 1, LowPrivilege, StandardRequireCall},
    {"title", 1, MediumPrivilege, StandardTitleCall},
    {"interrupt", 1, MediumPrivilege, StandardInterruptCall},
    {"push", 1, MediumPrivilege, StandardPushCall},
    {"pop", 0, MediumPrivilege, StandardPopCall},
    {"create", 1, LowPrivilege, StandardCreateCall}
};