unsigned int GenerateSymbolHash(byte *symbol_name, short length) {
    unsigned int symbol_hash = 0;

    for(int i = 0; i < length; i++) {
        byte char_num = *(symbol_name + i);
        symbol_hash ^= (char_num * ((i + 1) * megascript_symbol_hash_frequency));
    }

    return symbol_hash;
}

byte CompareSymbolStrings(byte *symbol_name, byte *symbol_string, short length) {
    for(unsigned int j = 0; j < length; j++) {
        if(*(symbol_name + j) != *(symbol_string + j)) return 0;
    }
    return 1;
}

int64 GetScopeIndex(ScriptComponents*, int64);
void *ReadScriptMemoryRange(ScriptEnvironment*, Script*, uint64, uint64);

#define FindIsStrict 1U
#define FindTypedefsOnly 2U

int FindScriptTableValue(ScriptEnvironment *script_tasks, Script *subject_script, byte *symbol_name, byte byte_length, Symbol *script_table, int script_table_count, byte is_strict) {
    ScriptComponents script_values = subject_script->values;
    int ret_val = -1;
    short length = (short) byte_length + 1;
    unsigned int symbol_hash = GenerateSymbolHash(symbol_name, length);
    Scope current_scope = {0};
    current_scope.id = -1;
    if(is_strict) {
        int64 scope_index = GetScopeIndex(&script_values, script_values.scope); //white board over explaining meme
        if(scope_index != -1) {
            current_scope = *(script_values.scopes + scope_index);
        }
    }

    for(int i = 0; i < script_table_count; i++) {
        Symbol *current_symbol = (script_table + i);
        if(symbol_hash == current_symbol->hash && byte_length == current_symbol->length) {
            byte is_accessible = 0;
            if(is_strict & FindIsStrict) {
                Scope parent_scope = current_scope;
                if(current_symbol->scope == -1) {
                    is_accessible = 1;
                } else while(1) {
                    if(parent_scope.id == current_symbol->scope) {
                        is_accessible = 1;
                        break;
                    }
                    int64 scope_index = parent_scope.parent;
                    if(scope_index != -1 && script_values.scopes) {
                        parent_scope = *(script_values.scopes + GetScopeIndex(&script_values, scope_index));
                    } else if(scope_index == -1 && script_values.scopes) {
                        parent_scope.id = -1;
                        parent_scope.parent = -1;
                    } else {
                        break;
                    }
                }
                if(is_accessible) if(current_symbol->created_at > script_values.program_counter) {
                    is_accessible = 0;
                } //marker
            }
            if((!(is_strict & FindIsStrict) || is_accessible) && CompareSymbolStrings(symbol_name, ReadScriptMemoryRange(script_tasks, subject_script, current_symbol->name, current_symbol->length), length)) {
                if(is_strict & FindTypedefsOnly) {
                    if(current_symbol->type == CustomDataType) ret_val = i;
                } else {
                    ret_val = i;
                }
            }
        };
    }

    return ret_val;
}

int64 FindSymbol(ScriptEnvironment *script_tasks, Script *subject_script, byte *symbol_name, byte byte_length) {
    return FindScriptTableValue(script_tasks, subject_script, symbol_name, byte_length, subject_script->values.symbol_table, subject_script->values.symbol_count, 1);
}

int64 FindLabel(ScriptEnvironment *script_tasks, Script *subject_script, byte *symbol_name, byte byte_length) {
    return FindScriptTableValue(script_tasks, subject_script, symbol_name, byte_length, subject_script->values.symbol_table, subject_script->values.symbol_count, 0);
}

int64 FindTypedef(ScriptEnvironment *script_tasks, Script *subject_script, byte *symbol_name, byte byte_length) {
    return FindScriptTableValue(script_tasks, subject_script, symbol_name, byte_length, subject_script->values.symbol_table, subject_script->values.symbol_count, FindIsStrict | FindTypedefsOnly);
}

int64 FindSymbolById(Script *subject_script, uint64 symbol_id) {
    for(int64 i = 0; i < subject_script->values.symbol_count; i++) {
        Symbol *current_symbol = subject_script->values.symbol_table + i;
        if(current_symbol->id == symbol_id) return i;
    }
    return -1;
}

int64 FindDebris(Script *subject_script, uint64 debris_id) {
    for(int64 i = 0; subject_script->system.debris_count; i++) {
        if((subject_script->system.debris + i)->id == debris_id) return i;
    }
    return -1;
}

int64 CreateScriptTableValue(ScriptEnvironment *script_tasks, Script *subject_script, Symbol new_symbol, Symbol *symbol_table, long long unsigned int *value_count) {
    new_symbol.id = script_tasks->system.symbol_id_auto_increment++;
    byte *memory_buffer = ReadScriptMemoryRange(script_tasks, subject_script, new_symbol.name, new_symbol.length);
    if(!memory_buffer) return -1;
    new_symbol.hash = GenerateSymbolHash(memory_buffer, (short) new_symbol.length + 1);
    new_symbol.scope = subject_script->values.scope;
    *(symbol_table + *value_count) = new_symbol;
    return (*value_count)++;
}

int64 CreateSymbol(ScriptEnvironment *script_tasks, Script *subject_script, Symbol new_symbol) {
    subject_script->values.symbol_table = realloc(subject_script->values.symbol_table, (subject_script->values.symbol_count + 1) * sizeof(Symbol));
    return CreateScriptTableValue(script_tasks, subject_script, new_symbol, subject_script->values.symbol_table, &subject_script->values.symbol_count);
}

int64 CreateLabel(ScriptEnvironment *script_tasks, Script *subject_script, Symbol new_symbol) {
    subject_script->values.symbol_table = realloc(subject_script->values.symbol_table, (subject_script->values.symbol_count + 1) * sizeof(Symbol));
    return CreateScriptTableValue(script_tasks, subject_script, new_symbol, subject_script->values.symbol_table, &subject_script->values.symbol_count);
}

int64 CreateDebris(ScriptEnvironment *script_tasks, Script *subject_script, TemporarySymbol *debris_symbol) {
    debris_symbol->id = script_tasks->system.symbol_id_auto_increment++;
    //debris_symbol->settings |= AutoFreeMemory; //For use only when opening new memory for a new value
    subject_script->system.debris = realloc(subject_script->system.debris, (subject_script->system.debris_count + 1) * sizeof(TemporarySymbol));
    *(subject_script->system.debris + subject_script->system.debris_count) = *debris_symbol;
    return subject_script->system.debris_count++;
}

TemporarySymbol RemoveDebris(ScriptEnvironment *script_tasks, Script *subject_script, int64 symbol_index) {
    TemporarySymbol removing_debris = {0};
    if(subject_script->system.debris_count > 0) {
        removing_debris = *(subject_script->system.debris + symbol_index);
    } else {
        return removing_debris;
    }
    RemoveArrayMember((byte**) &subject_script->system.debris, &subject_script->system.debris_count, symbol_index, sizeof(TemporarySymbol));
    return removing_debris;
}

int DeleteDebris(ScriptEnvironment *script_tasks, Script *subject_script, int64 symbol_index) {
    TemporarySymbol removing_debris = RemoveDebris(script_tasks, subject_script, symbol_index);
    //            printf("hi\n");
    if(removing_debris.settings & AutoFreeMemory && !(removing_debris.settings & DoNotFreeSymbolAddress)) {
        if(removing_debris.settings & AutoFreePointer) {
            uint64 *pointer = ReadScriptMemoryRange(script_tasks, subject_script, removing_debris.address, bit_mode / 8); //pointer marker
            if(pointer) {
                int error_code = FreeScriptMemory(script_tasks, subject_script, *pointer);
                if(error_code) return error_code;
            }
        }
        return FreeScriptMemory(script_tasks, subject_script, removing_debris.address);
    }
    return 0;
}

int FreeScriptMemory(ScriptEnvironment*, Script*, uint64);
//int a = 0 function aaaa() print(a) int b stop #meow aaaa(a = a + 1) jump @meow
int DeleteSymbol(ScriptEnvironment *script_tasks, Script *subject_script, int64 symbol_index) {
    byte symbol_type = 0;
    uint64 address = 0;
    short symbol_settings = 0;
    if(subject_script->values.symbol_count <= 0) return CouldNotDeleteSymbolError;
    Symbol *current_symbol = subject_script->values.symbol_table + symbol_index;
    byte do_not_free = (current_symbol->settings & DoNotFreeSymbolAddress) != 0;
    address = current_symbol->address;
    symbol_settings = current_symbol->settings;
    symbol_type = current_symbol->type;
    if(RemoveArrayMember((byte**) &subject_script->values.symbol_table, &subject_script->values.symbol_count, symbol_index, sizeof(Symbol))) return CouldNotDeleteSymbolError;
    
    if(symbol_settings & AutoFreeMemory) {
        byte multiply_factor = bit_mode / 8;
        byte *data_buffer = 0;
        switch(symbol_type) {
            case EnumType:
                data_buffer = ReadScriptMemoryRange(script_tasks, subject_script, address, multiply_factor + multiply_factor);
                if(data_buffer) {
                    int error_code = 0;
                    if(bit_mode == 32) {
                        uint32 address_32 = *(uint32*) data_buffer;
                        if(error_code = FreeScriptMemory(script_tasks, subject_script, address_32)) return error_code;
                        address_32 = *(uint32*) (data_buffer + multiply_factor);
                        if(error_code = FreeScriptMemory(script_tasks, subject_script, address_32)) return error_code;
                    } else if(bit_mode == 64) {
                        uint64 address_64 = *(uint64*) data_buffer;
                        if(error_code = FreeScriptMemory(script_tasks, subject_script, address_64)) return error_code;
                        address_64 = *(uint64*) (data_buffer + multiply_factor);
                        if(error_code = FreeScriptMemory(script_tasks, subject_script, address_64)) return error_code; //marker: this might be causing errors
                    }
                }
        }
    }
    if(do_not_free) return 0;
    return FreeScriptMemory(script_tasks, subject_script, address);
}

byte ReadScriptMemory(ScriptEnvironment*, Script*, uint64);

byte *GetSymbolCodeString(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    byte *ret_val = 0;
    uint64 starting_point = *pc;
    byte byte_length = ReadScriptMemory(script_tasks, subject_script, *pc);
    (*pc)++;
    uint64 length = (short) byte_length + 1;
    byte *string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, *pc, length);
    if(string_pointer) {
        *pc += length;
        ret_val = script_tasks->memory + starting_point;
    }

    return ret_val;
}

TemporarySymbol ConvertSymbolToDebris(Symbol current_symbol) {
    TemporarySymbol converted_symbol = {0};
    converted_symbol.type = current_symbol.type;
    converted_symbol.sub_type = current_symbol.sub_type;
    converted_symbol.size = current_symbol.size;
    converted_symbol.list_pointer = current_symbol.list_pointer;
    converted_symbol.settings = current_symbol.settings;
    converted_symbol.id = current_symbol.id;
    converted_symbol.address = current_symbol.address;
    converted_symbol.dimension = current_symbol.dimension;
    converted_symbol.array_dimension = current_symbol.array_dimension;
    return converted_symbol;
}