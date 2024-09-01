uint64 GetPointerFromMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *address) {
    byte *pointer = 0;
    switch(bit_mode) {
        case 64:
            pointer = ReadScriptMemoryRange(script_tasks, subject_script, *address, 8);
            if(pointer) {
                *address += 8;
                return *(uint64*) pointer;
            }
            break;
        case 32:
            pointer = ReadScriptMemoryRange(script_tasks, subject_script, *address, 4);
            if(pointer) {
                *address += 4;
                return (uint64) *(unsigned int*) pointer;
            }
            break;
    }
    return 0;
}

int WriteStackDataToMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, StackData data_value) {
    int error_code = 0;
    int multiply_factor = bit_mode / 8;
    if(!data_value.data) return MissingStackDataError;
    if(data_value.type == TemporarySymbolType) data_value = ExpandDebris(script_tasks, subject_script, data_value);
    CalculationValues values = {0};
    switch(data_value.type) {
        case StringType:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, StringOpcode)) return error_code;
            address++;
            uint64 string_length = data_value.length;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, multiply_factor, &string_length)) return error_code;
            address += multiply_factor;
            byte *string_buffer = data_value.data;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, string_length, string_buffer)) return error_code;
            return 0;
        case UnsignedInt8Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, IntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 1)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, *data_value.data)) return error_code;
            return 0;
        case Int8Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, SignedIntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 1)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, *data_value.data)) return error_code;
            return 0;
        case UnsignedInt16Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, IntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 2)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 2, data_value.data)) return error_code;
            return 0;
        case Int16Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, SignedIntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 2)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 2, data_value.data)) return error_code;
            return 0;
        case UnsignedInt32Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, IntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 4)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 4, data_value.data)) return error_code;
            return 0;
        case Int32Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, SignedIntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 4)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 4, data_value.data)) return error_code;
            return 0;
        case UnsignedInt64Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, IntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 8)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 8, data_value.data)) return error_code;
            return 0;
        case Int64Type:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, SignedIntegerOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, 8)) return error_code;
            address++;
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, 8, data_value.data)) return error_code;
            return 0;
        case CharType:
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, CharacterOpcode)) return error_code;
            address++;
            if(error_code = WriteScriptMemory(script_tasks, subject_script, address, *data_value.data)) return error_code;
            return 0;
    }
    return UnsupportedStackDataTypeError;
}

int SymbolConstructionLoop(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc, uint64 *create_variable, StackData *stack_data, byte *data_type, uint64 **array_initializer_array, uint64 *array_initializer_count, uint64 *dereference_count) {
    int return_code = 0;
    byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
    byte looping = 1;
    StackData ret_val = {0};
    int multiply_factor = bit_mode / 8;
    while(looping) {
        switch(bytecode) {
            case CreateVariableOpcode:
                *create_variable = *pc;
                *stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
                ret_val = *stack_data;
                *data_type = stack_data->type;
                break;
            case CreateStructElementSymbolConstructorCode:
                return_code = -1;
                (*pc)++;
                stack_data->type = ReadScriptMemory(script_tasks, subject_script, *pc);
                (*pc)++;
                if(stack_data->type == CustomDataType) {
                    byte *temp_token = GetSymbolCodeString(script_tasks, subject_script, pc);
                    byte byte_length = *temp_token;
                    temp_token++;
                    int64 typedef_index = FindTypedef(script_tasks, subject_script, temp_token, byte_length);
                    if(typedef_index != -1) {
                        Symbol *temp_symbol = subject_script->values.symbol_table + typedef_index;
                        stack_data->type = temp_symbol->sub_type;
                        *create_variable = temp_symbol->list_pointer;
                    } else {
                        return TypedefNotFoundError;
                    }
                }
                stack_data->data = GetSymbolCodeString(script_tasks, subject_script, pc);
                ret_val = *stack_data;
                *data_type = stack_data->type;
                break;
            case ArrayInitializerSymbolConstructorCode:
                (*pc)++;
                *array_initializer_array = realloc(*array_initializer_array, (*array_initializer_count + 1) * multiply_factor);
                stack_data->data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, multiply_factor);
                if(!stack_data->data) {
                    free(*array_initializer_array);
                    return ArrayInitializerMemoryReadError;
                }
                *pc += multiply_factor;
                *(*array_initializer_array + *array_initializer_count) = *(uint64*) stack_data->data;
                (*array_initializer_count)++;
                break;
            case PointerMarkerSymbolConstructorCode:
                (*pc)++;
                (*dereference_count)++;
                break;
            case EndSymbolConstructorCode:
                (*pc)++;
            default:
                looping = 0;
        }
        bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
    }
    *stack_data = ret_val;
    return return_code;
}

StackData GetStackDataFromMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    StackData ret_val = {0};
    byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
    byte *data_pointer = 0;
    uint64 data_length = 0;
    uint16 bit16 = 0;
    int data_value = 0;
    int multiply_factor = bit_mode / 8;
    switch(bytecode) {
        case StringOpcode:
            (*pc)++;
            data_length = GetPointerFromMemory(script_tasks, subject_script, pc);
            if(data_length) {
                data_pointer = ReadScriptMemoryRange(script_tasks, subject_script, *pc, data_length);
                if(data_pointer) {
                    ret_val.data = data_pointer;
                    ret_val.type = StringType;
                    ret_val.length = data_length;
                    *pc += data_length;
                }
            }
            break;
        case CharacterOpcode:
            (*pc)++;
            ret_val.type = CharType;
            ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, 1);
            ret_val.length = 1;
            (*pc)++;
            break;
        case SignedIntegerOpcode:
            (*pc)++;
            data_length = ReadScriptMemory(script_tasks, subject_script, *pc);
            (*pc)++;
            ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, data_length);
            if(!ret_val.data) {
                ret_val.error = 1;
                return ret_val;
            }
            *pc += data_length;
            switch(data_length) {
                case 0:
                    ret_val.type = VoidType;
                    break;
                case 1:
                    ret_val.type = Int8Type;
                    break;
                case 2:
                    ret_val.type = Int16Type;
                    break;
                case 4:
                    ret_val.type = Int32Type;
                    break;
                case 8:
                    ret_val.type = Int64Type;
                    break;
            }
            break;
        case IntegerOpcode:
            (*pc)++;
            data_length = ReadScriptMemory(script_tasks, subject_script, *pc);
            (*pc)++;
            ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, data_length);
            if(!ret_val.data) {
                ret_val.error = 1;
                return ret_val;
            }
            *pc += data_length;
            switch(data_length) {
                case 0:
                    ret_val.type = VoidType;
                    break;
                case 1:
                    ret_val.type = UnsignedInt8Type;
                    break;
                case 2:
                    ret_val.type = UnsignedInt16Type;
                    break;
                case 4:
                    ret_val.type = UnsignedInt32Type;
                    break;
                case 8:
                    ret_val.type = UnsignedInt64Type;
                    break;
            }
            break;
        case FloatOpcode:
            (*pc)++;
            data_length = ReadScriptMemory(script_tasks, subject_script, *pc);
            (*pc)++;
            ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, data_length);
            if(!ret_val.data) {
                ret_val.error = 1;
                return ret_val;
            }
            *pc += data_length;
            switch(data_length) {
                case 0:
                    ret_val.type = VoidType;
                    break;
                case 4:
                    ret_val.type = FloatType;
                    break;
                case 8:
                    ret_val.type = DoubleType;
                    break;
                case 16:
                    ret_val.type = LongDoubleType;
                    break;
            }
            break;
        case EnumOpcode:
            (*pc)++;
            ret_val.type = EnumType;
            ret_val.data = GetSymbolCodeString(script_tasks, subject_script, pc);
            if(!ret_val.data) ret_val.error = EnumDefiningError;
        case FunctionOpcode:
            (*pc)++;
            ret_val.type = FunctionType;
            ret_val.data = GetSymbolCodeString(script_tasks, subject_script, pc);
            if(!ret_val.data) ret_val.error = SymbolDefiningError;
            break;
        case LambdaFunctionOpcode:
            (*pc)++;
            ret_val.type = FunctionType;
            ret_val.data = script_tasks->memory + *pc;
            if(!ret_val.data) ret_val.error = LambdaFunctionDefiningError;
            break;
        case EnumElementOpcode:
        case VariableOpcode:
            (*pc)++;
            ret_val.type = SymbolType;
            ret_val.data = GetSymbolCodeString(script_tasks, subject_script, pc);
            if(!ret_val.data) ret_val.error = SymbolDefiningError;
            break;
        case ElementOpcode:
            (*pc)++;
            ret_val.type = ElementType;
            ret_val.data = malloc(bit_mode / 8);
            memcpy(ret_val.data, pc, bit_mode / 8);
            GetSymbolCodeString(script_tasks, subject_script, pc);
            if(!ret_val.data) ret_val.error = SymbolDefiningError;
            break;
        case CreateVariableOpcode:
            (*pc)++;
            ret_val.type = ReadScriptMemory(script_tasks, subject_script, *pc);
            (*pc)++;
            if(ret_val.type == CustomDataType) {
                byte *temp_token = GetSymbolCodeString(script_tasks, subject_script, pc);
                byte byte_length = *temp_token;
                temp_token++;
                int64 typedef_index = FindTypedef(script_tasks, subject_script, temp_token, byte_length);
                if(typedef_index != -1) {
                    Symbol *temp_symbol = subject_script->values.symbol_table + typedef_index;
                    ret_val.type = temp_symbol->sub_type;
                } else {
                    ret_val.error = TypedefNotFoundError;
                    return ret_val;
                }
            }
            ret_val.data = GetSymbolCodeString(script_tasks, subject_script, pc);
            if(!ret_val.data) ret_val.error = SymbolDefiningError;
            break;
        case StandardCallOpcode:
            (*pc)++;
            ret_val.type = UnsignedInt16Type;
            ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, *pc, 2);
            *pc += 2;
            break;
        case DataTypeOpcode:
            (*pc)++;
            ret_val.type = DataType;
            byte *data_type = ReadScriptMemoryRange(script_tasks, subject_script, *pc, 1);
            ret_val.data = data_type;
            (*pc)++;
            break;
        case NewOpcode:
            *pc += 2;
            break;
        case ConstructSymbolOpcode:
            (*pc)++;
            uint64 create_variable = 0;
            uint64 dereference_count = 0;
            uint64 array_initializer_count = 0;
            uint64 *array_initializer_array = 0; //pointer marker
            byte data_type_2 = 0;
            ret_val.error = SymbolConstructionLoop(script_tasks, subject_script, pc, &create_variable, &ret_val, &data_type_2, &array_initializer_array, &array_initializer_count, &dereference_count);
            break;
        default:
            (*pc)++;
    }
    return ret_val;
}

int AllocateMegaScriptEnvironmentMemory(ScriptEnvironment *script_tasks, uint64 allocate_memory) {
    int error_code = 0;
    script_tasks->memory = realloc(script_tasks->memory, allocate_memory);
    script_tasks->memory_size = allocate_memory;
    return error_code;
}

int64 InspectMemorySectionChunk(MemorySection *section_chunk, uint64 pointer, uint64 search_length, uint64 offset) {
    int64 ret_val = -1;
    for(int i = 0; i < search_length; i++) {
        MemorySection *current_selection = (section_chunk + i);
        if(pointer >= current_selection->pointer && pointer <= current_selection->pointer + (current_selection->length - 1)) {
            ret_val = i + offset;
        }
    }
    return ret_val;
}

int64 FindMemorySection(MemoryManager *manager, uint64 pointer) {
    int64 ret_val = -1;
    uint64 count = manager->count;
    if(manager->count > 0 && manager->sections != 0) {
        MemorySection *last_chunk;
        uint64 offset = 0;
        byte broken_loop = 0;
        for(int i = 0; i < (count - 1) / megascript_memory_search_gap; i += megascript_memory_search_gap) {
            if((manager->sections + i)->pointer > pointer) {
                broken_loop = 1;
                break;
            }
            last_chunk = manager->sections + i;
            offset = i;
        }
        if((count - 1) / megascript_memory_search_gap) ret_val = InspectMemorySectionChunk(last_chunk, pointer, megascript_memory_search_gap, offset);
        if(ret_val == -1 && !broken_loop) ret_val = InspectMemorySectionChunk(manager->sections + offset, pointer, count - offset, offset);
    }

    return ret_val;
}

uint64 ClaimMemorySection(ScriptEnvironment *script_tasks, Script *subject_script, uint64 allocate_memory) {
    if(!allocate_memory) return AttemptedToAllocateZeroError;
    while(script_tasks->memory_manager.busy) WaitOSTime(1);
    script_tasks->memory_manager.busy = 1; //this is important if we want to split script execution between two threads
    int error_code = 0;
    uint64 pointer = 0;
    uint64 post_pointer = 0;
    MemorySection current_selection = {0};
    current_selection.pointer = script_tasks->memory_reserve;
    current_selection.length = 0;
    MemoryManager *manager = &script_tasks->memory_manager;
    MemorySection new_section = {0};
    new_section.owner = subject_script;
    new_section.length = allocate_memory;

    for(int i = 0; i < manager->count; i++) {
        MemorySection post_selection = *(script_tasks->memory_manager.sections + i);
        uint64 entry_point = current_selection.pointer + current_selection.length;
        uint64 ending_point = post_selection.pointer;
        post_pointer = ending_point + post_selection.length;
        uint64 space_length = ending_point - entry_point;
        if(allocate_memory <= space_length) {
            pointer = entry_point;
            new_section.pointer = pointer;
            if(InsertArrayMember((byte**) &manager->sections, (uint64*) &manager->count, (uint64) i, sizeof(MemorySection), (void*) &new_section)) {
                script_tasks->memory_manager.busy = 0;
                return 0;
            }
            break;
        } else {
            current_selection = post_selection;
        }
    }

    if(!pointer && manager->count <= 0 && script_tasks->memory_size > script_tasks->memory_reserve + allocate_memory) {
        pointer = script_tasks->memory_reserve;
        manager->count += 1;
        manager->sections = realloc(manager->sections, manager->count * sizeof(MemorySection));
        new_section.pointer = pointer;
        *(manager->sections + (manager->count - 1)) = new_section;
    } else if(!pointer && script_tasks->max_memory >= script_tasks->memory_size + allocate_memory) {
        uint64 new_pointer = post_pointer; //add an option for dynamic memory resizing
        error_code = AllocateMegaScriptEnvironmentMemory(script_tasks, script_tasks->memory_size + allocate_memory); //add this back once section pointers are changed to ints
        if(!error_code) {
            manager->count += 1;
            manager->sections = realloc(manager->sections, manager->count * sizeof(MemorySection));
            new_section.pointer = new_pointer;
            pointer = new_pointer;
            *(manager->sections + (manager->count - 1)) = new_section;
        }
    }

    script_tasks->memory_manager.busy = 0;

    return pointer;
}

uint64 GetMemoryDataSize(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *address) {
    int error_code = 0;
    uint64 data_length = *address;
    StackData new_sd = GetStackDataFromMemory(script_tasks, subject_script, address);
    switch(new_sd.type) {
        case FunctionType:
            if(error_code = SkipScopeData(script_tasks, subject_script, address)) return error_code;
            (*address)++;
    }
    return *address - data_length;
}

int DeleteMemorySection(ScriptEnvironment *script_tasks, Script *subject_script, uint64 pointer) {
    int error_code = 0;
    MemoryManager *manager = &script_tasks->memory_manager;
    MemorySection *sections = manager->sections;
    if(!pointer) return 0;
    while(script_tasks->memory_manager.busy) WaitOSTime(1);
    script_tasks->memory_manager.busy = 1; //this is important if we want to split script execution between two threads

    if(manager->count < 1 || manager->sections == 0) {
        error_code = NoMemorySectionsError;
    } else {
        int64 index = FindMemorySection(manager, pointer);
        if(index > -1) {
            MemorySection *current_section = sections + index;
            if(subject_script && current_section->owner != subject_script && subject_script->rules.settings & MemoryProtectionOn) {
                script_tasks->memory_manager.busy = 0;
                return MemoryNotOwnedError;
            }
            RemoveArrayMember((unsigned char**) &manager->sections, (unsigned long long int*) &manager->count, index, sizeof(MemorySection));
            if(manager->count <= index) {
                uint64 desired_size = 0;
                uint64 current_size = script_tasks->memory_size;
                if(manager->count <= 0) {
                    desired_size = script_tasks->min_memory;
                } else {
                    MemorySection *current_subject = manager->sections + (manager->count - 1);
                    desired_size = script_tasks->memory_size - current_subject->length;
                    if(desired_size < script_tasks->min_memory) desired_size = script_tasks->min_memory;
                }
                if(desired_size < script_tasks->memory_reserve) desired_size = script_tasks->memory_reserve;
                if(desired_size < current_size) {
                    if(desired_size < script_tasks->min_memory) desired_size = script_tasks->min_memory;
                    error_code = AllocateMegaScriptEnvironmentMemory(script_tasks, desired_size);
                }
            }
        } else {
            error_code = MemorySectionNotFoundError;
        }
    }

    script_tasks->memory_manager.busy = 0;

    return error_code;
}

#define CheckIfReadOnlyInRead 1U

void *ReadScriptMemoryRangeWithOptions(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, uint64 size, byte settings) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) return (void*) address;
    if(address + size < script_tasks->memory_size) {
        if(address < script_tasks->memory_reserve || CanIgnoreMemoryProtection & subject_script->rules.settings || !subject_script) {
            return script_tasks->memory + address;
        } else {
            int64 index = FindMemorySection(&script_tasks->memory_manager, address);
            if(index > -1) {
                MemorySection section = *(script_tasks->memory_manager.sections + index);
                if(section.owner->rules.settings & IsReadOnly && settings & CheckIfReadOnlyInRead) return 0;
                if((section.owner == subject_script || !(section.owner->rules.settings & MemoryProtectionOn)) && size <= section.length - (address - section.pointer)) {
                    return script_tasks->memory + address;
                }
            }
        }
    }
    return 0;
}

void *ReadScriptMemoryRangeCheckIfReadOnly(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, uint64 size) {
    return ReadScriptMemoryRangeWithOptions(script_tasks, subject_script, address, size, CheckIfReadOnlyInRead);
}

void *ReadScriptMemoryRange(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, uint64 size) {
    return ReadScriptMemoryRangeWithOptions(script_tasks, subject_script, address, size, 0);
}

byte ReadScriptMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) return *(byte*) address;
    if(address < script_tasks->memory_size) {
        if(address < script_tasks->memory_reserve || CanIgnoreMemoryProtection & subject_script->rules.settings || !subject_script) {
            return *(script_tasks->memory + address);
        } else {
            int64 index = FindMemorySection(&script_tasks->memory_manager, address);
            if(index > -1) {
                MemorySection section = *(script_tasks->memory_manager.sections + index);
                if((section.owner == subject_script || !(section.owner->rules.settings & MemoryProtectionOn))) {
                    return *(script_tasks->memory + address);
                }
            }
        }
    }
    return 0;
}

int WriteScriptMemoryRange(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, uint64 size, void *write) {
    if(!size) return 0;
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) {
        memcpy((void*) address, write, size);
        return 0;
    }
    if(address + size < script_tasks->memory_size) {
        if(address < script_tasks->memory_reserve || CanIgnoreMemoryProtection & subject_script->rules.settings || !subject_script) {
            memcpy(script_tasks->memory + address, write, size);
            return 0;
        } else {
            int64 index = FindMemorySection(&script_tasks->memory_manager, address);
            if(index > -1) {
                MemorySection section = *(script_tasks->memory_manager.sections + index);
                if((section.owner == subject_script || !(section.owner->rules.settings & MemoryProtectionOn)) && size <= section.length - (address - section.pointer)) {
                    if(section.owner->rules.settings & IsReadOnly) return ScriptIsReadOnlyError;
                    memcpy(script_tasks->memory + address, write, size);
                    return 0;
                }
            }
        }
    }

    return WriteFailureError;
}

int WriteScriptMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, byte byte1) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) {
        *(byte*) address = byte1;
        return 0;
    }
    if(address < script_tasks->memory_size) {
        //do script task error callback later on if it fails MemoryOutOfBoundsError
        if(address < script_tasks->memory_reserve || CanIgnoreMemoryProtection & subject_script->rules.settings || !subject_script) {
            *(script_tasks->memory + address) = byte1;
            return 0;
        } else {
            int64 index = FindMemorySection(&script_tasks->memory_manager, address);
            if(index > -1) {
                MemorySection section = *(script_tasks->memory_manager.sections + index);
                if((section.owner == subject_script || !(section.owner->rules.settings & MemoryProtectionOn))) {
                    if(section.owner->rules.settings & IsReadOnly) return ScriptIsReadOnlyError;
                    *(script_tasks->memory + address) = byte1;
                    return 0;
                }
            }
        }
    }

    return WriteFailureError;
}

uint64 AllocateScriptMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 allocate_memory) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) return (uint64) malloc(allocate_memory);
    return ClaimMemorySection(script_tasks, subject_script, allocate_memory);
}

int FreeScriptMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 memory_address) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) {
        free((void*) memory_address);
        return 0;
    }
    return DeleteMemorySection(script_tasks, subject_script, memory_address);
}

int ZeroScriptMemory(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address) {
    if(script_tasks->settings.bools & EnvironmentUsesRealAddresses) return 0;
    MemoryManager *mm = &script_tasks->memory_manager;
    int64 section_index = FindMemorySection(mm, address);
    if(section_index != -1) {
        MemorySection *section = mm->sections + section_index;
        if(!(section->owner->rules.settings & IsReadOnly)) {
            byte *memory_buffer = ReadScriptMemoryRange(script_tasks, subject_script, section->pointer, section->length);
            if(memory_buffer) {
                memset(memory_buffer, 0, section->length);
                return 0;
            }
        }
    }
    return 1;
}

uint64 CountScriptStatements(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address) {
    uint64 count = 1;
    uint64 pc = subject_script->values.program_starting_point;
    StackData current_data = {0};
    while(1) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, pc);
        switch(bytecode) {
            case EndStatementOpcode:
                pc++;
                count++;
                break;
            default:
                if(GetStackDataFromMemory(script_tasks, subject_script, &pc).error) return 0;
        }
        if(pc >= address) break;
    }
    return count;
}

