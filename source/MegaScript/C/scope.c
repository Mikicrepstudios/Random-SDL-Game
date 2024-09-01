int64 CreateScope(Script *subject_script, uint64 address, uint64 length, int64 parent) {
    subject_script->values.scopes = realloc(subject_script->values.scopes, (subject_script->values.scope_count + 1) * sizeof(Scope));
    Scope new_scope = {0};
    new_scope.id = subject_script->system.scope_id_auto_increment;
    new_scope.address = address;
    new_scope.length = length;
    new_scope.parent = parent;
    *(subject_script->values.scopes + subject_script->values.scope_count) = new_scope;
    subject_script->system.scope_id_auto_increment++;
    subject_script->values.scope_count++;
    return subject_script->values.scope_count - 1;
}

int64 GetScopeIndex(ScriptComponents *values, int64 search_id) {
    for(long long unsigned int i = 0; i < values->scope_count; i++) {
        Scope *current_scope = values->scopes + i;
        if(current_scope->id == search_id) return i;
    }
    return -1;
}

Scope *GetScope(Script *subject_script, int64 search_id) {
    Scope *ret_val = 0;
    if(search_id != -1) {
        int64 scope_index = GetScopeIndex(&subject_script->values, search_id);
        if(scope_index != -1) ret_val = subject_script->values.scopes + scope_index;
    }
    return ret_val;
}

int DeleteScope(ScriptEnvironment *script_tasks, Script *subject_script, int64 search_id) {
    uint64 scope_index = GetScopeIndex(&subject_script->values, search_id);
    if(scope_index == -1) return CouldNotFindScopeError;
    if(RemoveArrayMember((byte**) &subject_script->values.scopes, &subject_script->values.scope_count, scope_index, sizeof(Scope))) return CouldNotDeleteScopeError;
    byte looping = 1;
    while(looping) {
        looping = 0;
        for(uint64 i = 0; i < subject_script->values.symbol_count; i++) {
            Symbol current_symbol = *(subject_script->values.symbol_table + i);
            if(current_symbol.scope == search_id) {
                int error_code = DeleteSymbol(script_tasks, subject_script, i);
                if(error_code) return error_code;
                looping = 1;
                break;
            }
        }
    }
    return 0;
}

int64 GetScopeByAddress(Script *subject_script, uint64 starting_point) {
    uint64 parent_scope = -1;
    for(long long unsigned int i = 0; i < subject_script->values.scope_count; i++) {
        Scope iscope = *(subject_script->values.scopes + i);
        if(starting_point > iscope.address && iscope.address + iscope.length > starting_point) parent_scope = iscope.id; //marker just increase the range of the scope when appending code
    }
    return parent_scope;
}

StackData GetStackDataFromMemory(ScriptEnvironment*, Script*, uint64*);

int SkipScopeData(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    int ret_val = 0;
    int data_value = 1;
    while(data_value) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        StackData stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
        switch(bytecode) {
            case StopOpcode:
                data_value--;
                if(data_value == 0 && bytecode != StopOpcode) ret_val = bytecode;
                break;
            case FunctionOpcode:
            case LambdaFunctionOpcode:
            case IfOpcode:
            case WhileOpcode:
            case ForOpcode:
            case EnumOpcode:
                data_value++;
                break;
        }
    }
    return 0;
}

int SkipStatement(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    byte looping = 1;
    while(looping) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        StackData stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
        switch(bytecode) {
            case EndStatementOpcode:
            case CommaOperator:
            case 0:
                looping = 0;
        }
    }
    return 0;
}

int SmartSkipStatement(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    byte looping = 1;
    int call_brackets = 0;
    int array_brackets = 0;
    int dynamic_brackets = 0;
    while(looping) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        StackData stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
        switch(bytecode) {
            case CallOperator:
                call_brackets++;
                break;
            case ArrayOperator:
                array_brackets++;
                break;
            case DynamicOperator:
                dynamic_brackets++;
                break;
            case CallEndOperator:
                if(call_brackets > 0) {
                    call_brackets--;
                } else {
                    (*pc)--;
                    looping = 0;
                }
                break;
            case ArrayEndOperator:
                if(call_brackets > 0) {
                    call_brackets--;
                } else {
                    (*pc)--;
                    looping = 0;
                }
                break;
            case DynamicEndOperator:
                if(call_brackets > 0) {
                    call_brackets--;
                } else {
                    (*pc)--;
                    looping = 0;
                }
                break;
            case EndStatementOpcode:
            case 0:
                (*pc)--;
            case TernaryTwoOperator:
                looping = 0;
        }
    }
    return 0;
}


