
//█████ █   █ █   █  ████ █████ █████  ███  █   █ █████ 
//█     █   █ ██  █ █       █     █   █   █ ██  █ █     
//████  █   █ █ █ █ █       █     █   █   █ █ █ █ █████ 
//█     █   █ █  ██ █       █     █   █   █ █  ██     █ 
//█     █████ █   █  ████   █   █████  ███  █   █ █████ 

int CreateVariableTemplate(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc, byte data_type, Symbol *new_symbol, short settings) {
    new_symbol->type = data_type;
    int predetermined_size = 0;
    byte hell = 0;
    if(new_symbol->type == CustomDataType) {
        byte *temp_token = GetSymbolCodeString(script_tasks, subject_script, pc);
        byte byte_length = *temp_token;
        temp_token++;
        int64 typedef_index = FindTypedef(script_tasks, subject_script, temp_token, byte_length);
        hell++;
        if(typedef_index != -1) {
            Symbol *temp_symbol = subject_script->values.symbol_table + typedef_index;
            new_symbol->type = temp_symbol->sub_type;
            new_symbol->list_pointer = temp_symbol->list_pointer;
            if(temp_symbol->size) {
                new_symbol->size = temp_symbol->size;
                predetermined_size = 1;
            }
        } else {
            return TypedefNotFoundError;
        }
    }
    byte old_data_type = new_symbol->type;
    byte *address = GetSymbolCodeString(script_tasks, subject_script, pc);
    if(!address) return SymbolDefiningError;
    data_type = *address;
    new_symbol->length = data_type;
    address++;
    new_symbol->name = address - script_tasks->memory;
    int multiply_factor = bit_mode / 8;
    if(!predetermined_size) switch(old_data_type) {
        case StructType:
            uint64 *list_table = (uint64*) ReadScriptMemoryRange(script_tasks, subject_script, new_symbol->list_pointer, multiply_factor * 4);
            if(!list_table) return CouldNotReadMemoryError;
            uint64 items_count = *list_table;
            uint64 bytes = 0;
            for(uint64 i = 0; i < items_count; i++) {
                byte *data_ptr = ReadScriptMemoryRange(script_tasks, subject_script, *(list_table + 2) + i, 1);
                if(!data_ptr) return CouldNotReadMemoryError;
                switch(*data_ptr) {
                    case StructType:
                        uint64 *aux_ptr = ReadScriptMemoryRange(script_tasks, subject_script, *(list_table + 3) + (i * multiply_factor), multiply_factor);
                        int64 return_size = GetRecursiveStructSize(script_tasks, subject_script, *aux_ptr);
                        if(return_size == -1) return FailedToGetStructSizeError;
                        bytes += return_size;
                        break;
                    default:
                        bytes += GetDataTypeLength(*data_ptr);
                }
            }
            new_symbol->size = bytes;
            break;
        case Bool8Type:
        case Bool16Type:
        case Bool32Type:
        case Bool64Type:
            new_symbol->size = GetDataTypeSize(old_data_type);
            break;
        default:
            new_symbol->size = GetDataTypeLength(old_data_type);
    }
    switch(old_data_type) {
        case ObjectType:
            new_symbol->settings |= HasGeniunePointer;
            new_symbol->settings |= DoNotFreeSymbolAddress;
            break;
        default:
            new_symbol->address = AllocateScriptMemory(script_tasks, subject_script, new_symbol->size);
            ZeroScriptMemory(script_tasks, subject_script, new_symbol->address);
            new_symbol->settings |= AutoFreeMemory;
    }
    new_symbol->type = old_data_type;
    new_symbol->settings |= IsCLike;
    new_symbol->settings |= settings;
    return 0;
}

uint64 CountEnumCommas(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *pc) {
    uint64 comma_count = 0;
    int brackets = 0;
    byte looping = 1;
    while(looping) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        switch(bytecode) {
            case CallEndOperator:
            case ArrayEndOperator:
            case DynamicEndOperator:
                if(brackets == 0) looping = 0;
        }
        if(!looping) break;
        StackData stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
        switch(bytecode) {
            case CallOperator:
            case ArrayOperator:
            case DynamicOperator:
                brackets++;
                break;
            case CallEndOperator:
            case ArrayEndOperator:
            case DynamicEndOperator:
                brackets--;
                break;
            case EndStatementOpcode:
            case 0:
                looping = 0;
                break;
            case CommaOperator:
                if(brackets == 0) comma_count++;
        }
    }
    return comma_count;
}

int ExecuteMegaScriptOpcode(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    uint64 *pc = &subject_script->values.program_counter;
    uint64 data_address = 0;
    uint64 data_length = 0;
    byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
    byte data_type = 0;
    byte *address = 0;
    int data_value = 0;
    Symbol new_symbol = {0};
    Symbol *current_symbol = 0;
    byte *value = 0;
    StackData stack_data = {0};
    uint64 multiply_factor = bit_mode / 8;
    uint64 enum_data_pointer = 0;
    byte looping;
    uint64 symbol_list = 0; //pointer marker
    switch(bytecode) {
        case EndScriptOpcode:
            subject_script->values.state = FinishedState; //gotta fix the stupid nigger shit from running forever (it still doesnt work, fix later)
            if(CheckSetting(DeveloperNotifications)) printf("Script %d's state was set to Finished.\n", subject_script->id);
            return 0;
        case '\0':
            return NullOpcodeExecutionError;
        case FunctionOpcode:
            data_address = *pc;
            (*pc)++;
            address = GetSymbolCodeString(script_tasks, subject_script, pc);
            data_type = *address;
            address++;
            new_symbol.name = address - script_tasks->memory;
            new_symbol.length = data_type;
            new_symbol.type = PointerType;
            new_symbol.size = GetDataTypeLength(PointerType);
            new_symbol.address = AllocateScriptMemory(script_tasks, subject_script, new_symbol.size);
            value = ReadScriptMemoryRange(script_tasks, subject_script, new_symbol.address, new_symbol.size);
            if(value) {
                switch(new_symbol.size) {
                    case 4:
                        *(unsigned int*) value = (unsigned int) data_address;
                        break;
                    case 8:
                        *(uint64*) value = data_address;
                        break;
                }
            }
            CreateSymbol(script_tasks, subject_script, new_symbol);
            if(error_code = SkipScopeData(script_tasks, subject_script, pc)) return error_code;
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case LambdaFunctionOpcode:
            if(error_code = PushScriptStackPointer(subject_script, *pc)) return error_code;
            (*pc)++;
            error_code = SkipScopeData(script_tasks, subject_script, pc);
            break;
        case StringOpcode:
            if(script_tasks->settings.bools & StringsPointToCode) {
                PushScriptStackPointer(subject_script, *pc);
                GetStackDataFromMemory(script_tasks, subject_script, pc);
            } else {
                (*pc)++;
                data_length = GetPointerFromMemory(script_tasks, subject_script, pc);
                TemporarySymbol string_symbol = {0};
                string_symbol.settings |= IsCLike | AutoFreeMemory | AutoFreePointer;
                string_symbol.type = CPointerType;
                string_symbol.sub_type = CharType;
                string_symbol.dimension = 1;
                string_symbol.address = AllocateScriptMemory(script_tasks, subject_script, multiply_factor);
                if(!string_symbol.address) return FailedToAllocateMemoryError;
                data_address = AllocateScriptMemory(script_tasks, subject_script, data_length + 1);
                if(!data_address) return FailedToAllocateMemoryError;
                if(error_code = WriteScriptMemory(script_tasks, subject_script, data_address + data_length, 0)) return error_code;
                address = ReadScriptMemoryRange(script_tasks, subject_script, *pc, data_length);
                if(!address) return CouldNotReadMemoryError;
                if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, data_address, data_length, address)) return error_code;
                if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, string_symbol.address, multiply_factor, &data_address)) return error_code;
                if(-1 == CreateDebris(script_tasks, subject_script, &string_symbol)) return FailedToCreateDebrisError;
                error_code = PushScriptStackData(subject_script, (byte*) &string_symbol.id, TemporarySymbolType);
                *pc += data_length;
            }
            break;
        case IntegerOpcode:
        case SignedIntegerOpcode:
        case FloatOpcode:
        case CharacterOpcode:
        case DataTypeOpcode:
        case ElementOpcode:
            stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
            if(stack_data.error) return stack_data.error;
            error_code = PushScriptStackData(subject_script, stack_data.data, stack_data.type);
            break;
        case EnumElementOpcode:
            stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
            uint64 string_offset = stack_data.data - script_tasks->memory;
            StackData current_save = CheckScriptStackData(subject_script);
            uint64 original_point = subject_script->values.stack_pointer;
            while(current_save.type != EnumStackSaveType && current_save.type != NewDataType) {
                if(current_save.type == 0) return EnumElementBytecodeOutsideError;
                DecrementScriptStack(subject_script);
                current_save = CheckScriptStackData(subject_script);
            }
            switch(current_save.type) {
                case NewDataType:
                    DecrementScriptStack(subject_script);
                    current_save = CheckScriptStackData(subject_script);
                    if(!current_save.data) return CouldNotFindTheSymbolError;
                    subject_script->values.stack_pointer = original_point;
                    switch(current_save.type) {
                        case Bool8Type:
                        case Bool16Type:
                        case Bool32Type:
                        case Bool64Type:
                            enum_data_pointer = GetPointerFromStackData(current_save);
                            if(subject_script->system.enum_auto_increment >= GetDataTypeSize(current_save.type) * 8) return TooManyBoolElementsError;
                            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, enum_data_pointer + (multiply_factor * subject_script->system.enum_auto_increment), multiply_factor, (byte*) &string_offset)) return error_code;
                            subject_script->system.enum_auto_increment++;
                            error_code = PushScriptStackData(subject_script, 0, AutoDeleteType);
                            break;
                        default:
                            return UnsupportedNewDataTypeError;
                    }
                    break;
                case EnumStackSaveType:
                    subject_script->values.stack_pointer = original_point;
                    enum_data_pointer = GetPointerStackData(current_save);
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, enum_data_pointer + multiply_factor + (multiply_factor * subject_script->system.enum_auto_increment), multiply_factor, (byte*) &string_offset)) return error_code;
                    current_save.data += multiply_factor;
                    enum_data_pointer = GetPointerStackData(current_save);
                    if(ReadScriptMemory(script_tasks, subject_script, *pc) == AssignOperator) {
                        (*pc)++;
                        if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, IsFunctionArgument | StopAtComma)) return error_code;
                        StackData enum_data = PopScriptStackData(subject_script);
                        if(enum_data.error) return enum_data.error;
                        subject_script->system.enum_value_increment = CastToUnsignedInt64(enum_data); //pointer marker
                    }
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, enum_data_pointer + multiply_factor + (multiply_factor * subject_script->system.enum_auto_increment), multiply_factor, (byte*) &subject_script->system.enum_value_increment)) return error_code;
                    subject_script->system.enum_auto_increment++;
                    subject_script->system.enum_value_increment++;
                    error_code = PushScriptStackData(subject_script, 0, AutoDeleteType);
            }
            break;
        case NewOpcode:
            new_symbol.created_at = *pc;
            (*pc)++;
            data_type = ReadScriptMemory(script_tasks, subject_script, *pc);
            (*pc)++;
            byte operation_settings = 0;
            uint64 position = *pc;
            uint64 items_count = 0;
            byte push_type = ListType;
            switch(data_type) {
                case Bool8Type:
                case Bool16Type:
                case Bool32Type:
                case Bool64Type:
                    push_type = data_type;
                    if(error_code = CreateVariableTemplate(script_tasks, subject_script, pc, data_type, &new_symbol, 0)) return error_code;
                    position = *pc;
                    uint64 additional_add = GetDataTypeSize(data_type);
                    new_symbol.settings |= AutoFreeMemory;
                    new_symbol.type = CustomDataType;
                    new_symbol.sub_type = data_type;
                    items_count = additional_add * 8;
                    uint64 list_size = items_count  * multiply_factor;
                    symbol_list = AllocateScriptMemory(script_tasks, subject_script, list_size);
                    new_symbol.list_pointer = symbol_list;
                    new_symbol.address = symbol_list;
                    CalculationValues temp_values = {0};
                    break;
                case StructType:
                    byte *temp_name = GetSymbolCodeString(script_tasks, subject_script, pc);
                    new_symbol.length = *temp_name;
                    new_symbol.name = (temp_name + 1) - script_tasks->memory;
                    new_symbol.type = CustomDataType;
                    new_symbol.sub_type = StructType;
                    position = *pc;
                    if(!(symbol_list = AllocateScriptMemory(script_tasks, subject_script, multiply_factor * 4))) return FailedToAllocateStructListMemoryError;
                    new_symbol.list_pointer = symbol_list;
                    new_symbol.address = symbol_list;
                    uint64 comma_count = CountEnumCommas(script_tasks, subject_script, &position);
                    items_count = comma_count + 1;
                    if(WriteScriptMemoryRange(script_tasks, subject_script, symbol_list, multiply_factor, &items_count)) return FailedToSetTheStructMemberCountError;
                    operation_settings |= PrimitiveOnly;
                    uint64 list_member_1;
                    uint64 list_member_2;
                    uint64 list_member_3;
                    if(!(list_member_1 = AllocateScriptMemory(script_tasks, subject_script, items_count * multiply_factor))) return FailedToAllocateStructListMemoryError;
                    if(!(list_member_2 = AllocateScriptMemory(script_tasks, subject_script, items_count))) return FailedToAllocateStructListMemoryError;
                    if(!(list_member_3 = AllocateScriptMemory(script_tasks, subject_script, items_count * multiply_factor))) return FailedToAllocateStructListMemoryError;
                    if(WriteScriptMemoryRange(script_tasks, subject_script, symbol_list + multiply_factor, multiply_factor, &list_member_1)) return FailedToSetTheStructElementsPointerError;
                    if(WriteScriptMemoryRange(script_tasks, subject_script, symbol_list + (multiply_factor * 2), multiply_factor, &list_member_2)) return FailedToSetTheStructDataTypesPointerError;
                    if(WriteScriptMemoryRange(script_tasks, subject_script, symbol_list + (multiply_factor * 3), multiply_factor, &list_member_3)) return FailedToSetTheStructAuxilaryPointerError;
                    break;
                default:
                    return ImproperUsageOfNewError;
            }
            data_value = CreateSymbol(script_tasks, subject_script, new_symbol);
            if(data_type != StructType) switch(bit_mode) {
                case 32:
                    uint32 symbol_list_32 = (uint32) symbol_list;
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, (byte*) &symbol_list_32));
                    break;
                case 64:
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, (byte*) &symbol_list));
            }
            if(data_value == -1) return SymbolDefiningError;
            if(PushScriptStackPointerNumber(subject_script, symbol_list, push_type)) return StackOverflowError;
            if(PushScriptStackData(subject_script, 0, NewDataType)) return StackOverflowError;
            if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, (uint32) operation_settings)) return error_code;
            break;
        case VariableOpcode:
            stack_data = GetStackDataFromMemory(script_tasks, subject_script, pc);
            bytecode = *stack_data.data;
            stack_data.data++;
            data_value = FindSymbol(script_tasks, subject_script, stack_data.data, bytecode);
            if(data_value == -1) return SymbolNotFoundError;
            current_symbol = subject_script->values.symbol_table + data_value;
            address = (byte*) &data_value;
            if(PushScriptStackSymbol(subject_script, current_symbol->id)) return StackOverflowError;
            break;
        case CreateVariableOpcode:
            new_symbol.created_at = *pc;
            (*pc)++;
            data_type = ReadScriptMemory(script_tasks, subject_script, *pc); //the create variable opcode needs its entire code to remain in the opcode execute function since there's no way to have the memory reader skip the opcodes without having an extra argument passed
            (*pc)++;
            new_symbol.type = data_type;
            if(error_code = CreateVariableTemplate(script_tasks, subject_script, pc, data_type, &new_symbol, 0)) return error_code;
            data_value = CreateSymbol(script_tasks, subject_script, new_symbol);
            current_symbol = subject_script->values.symbol_table + data_value;
            if(PushScriptStackSymbol(subject_script, current_symbol->id)) return StackOverflowError;
            break;
        case EnumOpcode:
            subject_script->system.enum_value_increment = 0;
            new_symbol.created_at = *pc;
            (*pc)++;
            new_symbol.settings |= AutoFreeMemory;
            if(error_code = CreateVariableTemplate(script_tasks, subject_script, pc, EnumType, &new_symbol, IsConstant)) return error_code;
            position = *pc;
            uint64 comma_count = CountEnumCommas(script_tasks, subject_script, &position);
            items_count = comma_count + 1;
            multiply_factor = bit_mode / 8;
            uint64 list_size = (items_count + 1) * multiply_factor;
            symbol_list = AllocateScriptMemory(script_tasks, subject_script, list_size);
            uint64 value_list = AllocateScriptMemory(script_tasks, subject_script, list_size);
            switch(bit_mode) {
                case 32:
                    uint32 symbol_list_32 = (uint32) items_count;
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, symbol_list, multiply_factor, (byte*) &symbol_list_32));
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, value_list, multiply_factor, (byte*) &symbol_list_32));
                    symbol_list_32 = (uint32) symbol_list;
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, (byte*) &symbol_list_32));
                    symbol_list_32 = (uint32) value_list;
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address + multiply_factor, multiply_factor, (byte*) &symbol_list_32));
                    break;
                case 64:
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, symbol_list, multiply_factor, (byte*) &items_count));
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, value_list, multiply_factor, (byte*) &items_count));
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, (byte*) &symbol_list));
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address + multiply_factor, multiply_factor, (byte*) &value_list));
            }
            data_value = CreateSymbol(script_tasks, subject_script, new_symbol);
            if(PushScriptStackData(subject_script, script_tasks->memory + new_symbol.address, EnumStackSaveType)) return StackOverflowError;
            if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, 0)) return error_code;
            break;
        case ConstructSymbolOpcode:
            new_symbol.created_at = *pc;
            (*pc)++;
            byte struct_element = 0;
            uint64 create_variable = 0;
            uint64 dereference_count = 0;
            uint64 array_initializer_count = 0;
            uint64 *array_initializer_array = 0; //pointer marker
            error_code = SymbolConstructionLoop(script_tasks, subject_script, pc, &create_variable, &stack_data, &data_type, &array_initializer_array, &array_initializer_count, &dereference_count);
            if(error_code == -1) {
                error_code = 0;
                struct_element = 1;
                if(array_initializer_count) {
                    free(array_initializer_array);
                    return InitializedArrayInStructError;
                }
            } else if(error_code) {
                free(array_initializer_array);
                return error_code;
            }
            uint64 ending_address = *pc;
            if(struct_element) {
                if(data_type == ObjectType) return ObjectInStructError;
                original_point = subject_script->values.stack_pointer;
                current_save = CheckScriptStackData(subject_script);
                while(current_save.type != NewDataType) {
                    if(current_save.type == 0) return EnumElementBytecodeOutsideError;
                    DecrementScriptStack(subject_script);
                    current_save = CheckScriptStackData(subject_script);
                }
                DecrementScriptStack(subject_script);
                current_save = CheckScriptStackData(subject_script);
                subject_script->values.stack_pointer = original_point;
                enum_data_pointer = GetPointerFromStackData(current_save); //pointer marker
                uint64 *actual_pointer = ReadScriptMemoryRange(script_tasks, subject_script, enum_data_pointer, multiply_factor * 4);
                if(actual_pointer) {
                    if(subject_script->system.enum_auto_increment >= *actual_pointer) return StructureAutoIncrementDesyncedError;
                    uint64 string_pointer = stack_data.data - script_tasks->memory;
                    error_code = WriteScriptMemoryRange(script_tasks, subject_script, *(actual_pointer + 1) + (subject_script->system.enum_auto_increment * multiply_factor), multiply_factor, &string_pointer);
                    if(error_code) return error_code;
                    error_code = WriteScriptMemoryRange(script_tasks, subject_script, *(actual_pointer + 2) + subject_script->system.enum_auto_increment, 1, &data_type);
                    if(error_code) return error_code;
                    error_code = WriteScriptMemoryRange(script_tasks, subject_script, *(actual_pointer + 3) + (subject_script->system.enum_auto_increment * multiply_factor), multiply_factor, &create_variable);
                    if(error_code) return error_code;
                    subject_script->system.enum_auto_increment++;
                    error_code = PushScriptStackData(subject_script, 0, AutoDeleteType);
                } else return MemoryReadError;
            } else {
                if(dereference_count == 0 && array_initializer_count == 0) {
                    *pc = create_variable;
                    if(error_code = ExecuteMegaScriptOpcode(script_tasks, subject_script)) return error_code;
                    *pc = ending_address;
                } else {
                    new_symbol.type = CPointerType;
                    new_symbol.settings |= IsCLike;
                    new_symbol.sub_type = data_type;
                    new_symbol.size = multiply_factor; //pointer marker
                    new_symbol.address = AllocateScriptMemory(script_tasks, subject_script, multiply_factor);
                    uint64 zero_address = 0;
                    if(WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, &zero_address)) {
                        free(array_initializer_array);
                        return CPointerZeroingError;
                    }
                    if(dereference_count) new_symbol.dimension = dereference_count;
                    if(array_initializer_count) {
                        //new_symbol.dimension++;
                        new_symbol.array_dimension = array_initializer_count;
                        new_symbol.settings |= IsArray;
                        new_symbol.settings |= AutoFreeMemory;
                        uint64 multiplier = 1;
                        if(array_initializer_count > 1) return MultiDimensionalArraysNotYetAddedError;
                        for(int i = 0; i < array_initializer_count; i++) {
                            uint64 index_value = *(array_initializer_array + i);
                            multiplier *= index_value;
                        }
                        int data_size = 0;
                        if(dereference_count) {
                            data_size = GetDataTypeLength(new_symbol.type);
                        } else {
                            data_size = GetDataTypeLength(new_symbol.sub_type);
                        }
                        if(!data_size) {
                            free(array_initializer_array);
                            return AttemptedArrayDataSizeZeroError;
                        }
                        uint64 data_array = AllocateScriptMemory(script_tasks, subject_script, data_size * multiplier);
                        if(!data_array) {
                            free(array_initializer_array);
                            return ArrayMemoryAllocationFailedError;
                        }
                        if(WriteScriptMemoryRange(script_tasks, subject_script, new_symbol.address, multiply_factor, &data_array)) {
                            free(array_initializer_array);
                            return ArrayPointerAssigningError;
                        }
                    }
                    new_symbol.length = *stack_data.data;
                    new_symbol.name = (stack_data.data + 1) - script_tasks->memory;
                    int64 symbol_index = CreateSymbol(script_tasks, subject_script, new_symbol);
                    if(symbol_index == -1) {
                        free(array_initializer_array);
                        return SymbolDefiningError;
                    }
                    current_symbol = subject_script->values.symbol_table + symbol_index;
                    if(error_code = PushScriptStackSymbol(subject_script, current_symbol->id)) {
                        free(array_initializer_array);
                        return error_code;
                    }
                    free(array_initializer_array);
                }
            }
            break;
        case EndStatementOpcode:
        case StopStandardCallOpcode:
            return error_code;
        default:
            return InvalidExecutionBytecodeError;
    }
    return error_code;
}

int PushScriptOperator(ScriptSystem *script_system, byte operator) {
    if(script_system->operator_max > script_system->operator_count) {
        *(script_system->operator_storage + script_system->operator_count) = operator;
        script_system->operator_count++;
        return 0;
    }
    return TooManyOperatorsError;
}

byte PopScriptOperator(ScriptSystem *script_system) {
    if(script_system->operator_count > 0) {
        script_system->operator_count--;
        return *(script_system->operator_storage + script_system->operator_count);
    }
    return 0;
}

byte CheckScriptOperator(ScriptSystem *script_system) {
    if(script_system->operator_count > 0) {
        return *(script_system->operator_storage + (script_system->operator_count - 1));
    }
    return 0;
}

int SwitchScriptOperator(ScriptEnvironment *script_tasks, Script *subject_script, byte operator, byte *previous, StackData *second_sd, unsigned short *operator_count, int *data_index, unsigned short *parameter_count, int *stack_items_2, byte associativity) {
    int error_code = 0;
    OperatorInformation *last_info = (script_tasks->operator_info + *previous);
    byte last_precedence = last_info->precedence;
    OperatorInformation *current_info = (script_tasks->operator_info + operator); //Prefixes need to have their own elseif because they're handled differently.
    byte current_precedence = current_info->precedence;
    switch(current_info->type) {
        case PrefixOperatorType:
        case SuffixOperatorType:
            if((current_precedence >= last_precedence || last_precedence == 0) && current_info->associativity == LeftToRight) {
                switch(current_info->type) {
                    case PrefixOperatorType:
                        if(error_code = ProcessScriptOperationPrefix(script_tasks, subject_script, operator)) return error_code;
                        break;
                    case SuffixOperatorType:
                        if(error_code = ProcessScriptOperationSuffix(script_tasks, subject_script, operator)) return error_code;
                }
            } else {
                if(error_code = PushScriptOperator(&subject_script->system, operator)) return error_code;
                (*operator_count)++;
            }
            break;
        case NormalOperatorType:
            if(current_precedence <= last_precedence && last_precedence != 0 && (last_info->associativity == LeftToRight || associativity == RightToLeft)) {
                if(error_code = CalculateScriptOperation(script_tasks, subject_script, *previous)) return error_code;
                byte *new_ptr = malloc(second_sd->length);
                memcpy(new_ptr, second_sd->data, second_sd->length);
                second_sd->data = new_ptr;
                if(error_code = PushScriptStackData(subject_script, second_sd->data, second_sd->type)) return error_code;
                free(second_sd->data);
                if(*previous != CommaOperator) (*parameter_count)--;
                (*stack_items_2)--;
                (*data_index)--;
            } else {
                byte *new_ptr = malloc(second_sd->length);
                /*if(second_sd->type == ItemCountType) {
                    memcpy(new_ptr, second_sd->data - (second_sd->length - 2), second_sd->length);
                    second_sd->data = new_ptr + (second_sd->length - 2);
                } else {
                    memcpy(new_ptr, second_sd->data, second_sd->length);
                    second_sd->data = new_ptr;
                }*/
                memcpy(new_ptr, second_sd->data, second_sd->length);
                second_sd->data = new_ptr;
                if(error_code = PushScriptStackData(subject_script, second_sd->data, second_sd->type)) return error_code;
                free(second_sd->data);
                (*data_index)--;
                if(*previous != 255) if(error_code = PushScriptOperator(&subject_script->system, *previous)) return error_code;
                (*operator_count)++;
            }
            *previous = operator;
            break;
        default:
            return InvalidOperatorTypeError;
    }
    return error_code;
}

int FinalizeScriptOperators(ScriptEnvironment *script_tasks, Script *subject_script, byte previous, unsigned short *parameter_count, unsigned short *operator_count, int *stack_items_2) {
    int error_code;
    OperatorInformation *previous_info = script_tasks->operator_info + previous;
    switch(previous_info->type) {
        case NormalOperatorType:
            if(error_code = CalculateScriptOperation(script_tasks, subject_script, previous)) return error_code;
            if(previous != CommaOperator) (*parameter_count)--;
            (*stack_items_2)--;
            (*operator_count)--;
            break;
    }
    return 0;
}

int AddToOperationList(Script *subject_script, OperatorInformation **inverted_operator_info, StackData *returning_value, OperatorInformation **inverted_previous_info, StackData **stack_array, int *stack_items, byte **operator_array, int *operator_items, byte inverted_operator, uint16 *operator_count, byte *inverted_previous) {
    int error_code = 0;
    switch((*inverted_operator_info)->type) {
        case NormalOperatorType:
            *returning_value = PopScriptStackData(subject_script);
            if(error_code = RefreshStackData(returning_value)) return error_code;
            *stack_array = realloc(*stack_array, (*stack_items + 1) * sizeof(StackData)); //check if it's right to left here, and execute if so. also make a previous
            *(*stack_array + *stack_items) = *returning_value;
            (*stack_items)++;
        case PrefixOperatorType:
        case SuffixOperatorType:
            *operator_array = realloc(*operator_array, *operator_items + 1);
            *(*operator_array + *operator_items) = inverted_operator;
            (*operator_items)++;
            (*operator_count)--;
            break;
    }
    return 0;
}

int AddOperatorToArray(Script *subject_script, OperatorInformation *operator_info, byte bytecode, uint16 *operator_count, uint64 *pc, byte *finish_up, uint32 operation_save) {
    int error_code = 0;
    switch(operator_info->bytecode_type) {
        case OperatorBytecodeType:
            switch(operator_info->type) {
                case NormalOperatorType:
                    error_code = PushScriptOperator(&subject_script->system, bytecode);
                    if(error_code) return error_code;
                    (*operator_count)++;
                    (*pc)++;
                    break;
                case ClosingBracketOperatorType:
                    if(!(operation_save & IsFunctionArgument)) (*pc)++;
                default:
                    *finish_up = 1;
            }
    }
    return error_code;
};

int CalculateOperationWithPrecedence(ScriptEnvironment *script_tasks, Script *subject_script, uint16 *operator_count, byte *inverted_previous, OperatorInformation **inverted_previous_info, StackData **stack_array, int *stack_items, byte **operator_array, int *operator_items, byte calculate_right_to_left) {
    int error_code = 0;
    byte last_left_to_right_precedence = 0;
    StackData returning_value = PopScriptStackData(subject_script);
    **stack_array = returning_value;
    while((*operator_count)) {
        byte inverted_operator = PopScriptOperator(&subject_script->system); //Maybe make this continue to loop until it finishes all the right-to-left operations.
        OperatorInformation *inverted_operator_info = script_tasks->operator_info + inverted_operator;
        *inverted_previous = CheckScriptOperator(&subject_script->system);
        *inverted_previous_info = script_tasks->operator_info + *inverted_previous;
        //printf("Operator: %lld\n", inverted_operator);
        switch(inverted_operator_info->associativity) {
            case LeftToRight:
                if(error_code = AddToOperationList(subject_script, &inverted_operator_info, &returning_value, inverted_previous_info, stack_array, stack_items, operator_array, operator_items, inverted_operator, operator_count, inverted_previous)) return error_code;
                *inverted_previous_info = inverted_operator_info;
                *inverted_previous = inverted_operator;
                last_left_to_right_precedence = inverted_operator_info->precedence;
                break;
            case RightToLeft:
                if(calculate_right_to_left && inverted_operator_info->precedence >= (*inverted_previous_info)->precedence && inverted_operator_info->precedence >= last_left_to_right_precedence) {
                    StackData *old_data = *stack_array + (*stack_items - 1); //marker: be sure to make it skip right-to-left operators when left-to-right is being processed
                    unsigned short oc = 0;
                    switch(inverted_operator_info->type) {
                        case NormalOperatorType:
                            if(error_code = PushScriptStackData(subject_script, returning_value.data, returning_value.type)) return error_code;
                            if(error_code = CalculateScriptOperation(script_tasks, subject_script, inverted_operator)) return error_code;
                            break;
                        case PrefixOperatorType:
                            if(error_code = PushScriptStackData(subject_script, returning_value.data, returning_value.type)) return error_code;
                            if(error_code = ProcessScriptOperationPrefix(script_tasks, subject_script, inverted_operator)) return error_code;
                            break;
                        case SuffixOperatorType:
                            if(error_code = PushScriptStackData(subject_script, returning_value.data, returning_value.type)) return error_code;
                            if(error_code = ProcessScriptOperationSuffix(script_tasks, subject_script, inverted_operator)) return error_code;
                    }
                    //printf("fuck! :D\n");
                    returning_value = PopScriptStackData(subject_script);
                    *old_data = returning_value;
                    *inverted_previous_info = script_tasks->operator_info + inverted_operator;
                    *inverted_previous = inverted_operator;
                    (*operator_count)--;
                } else {
                    if(error_code = AddToOperationList(subject_script, &inverted_operator_info, &returning_value, inverted_previous_info, stack_array, stack_items, operator_array, operator_items, inverted_operator, operator_count, inverted_previous)) return error_code;
                    *inverted_previous_info = script_tasks->operator_info + inverted_operator;
                    *inverted_previous = inverted_operator;
                }
        }
    }
    return error_code;
}

int CalculateStackOperations(ScriptEnvironment *script_tasks, Script *subject_script, uint16 *operator_count, uint32 *operation_save, byte has_return, byte ternary_one_activated, uint64 *global_return_address) {
    int error_code = 0;
    uint64 *pc = &subject_script->values.program_counter;
    byte first_iteration = 1;
    StackData *stack_saves = 0;
    uint16 save_count = 0;
    uint16 parameter_count = 0;
    uint16 standard_call_id = 0;
    uint16 bytecode_2 = 0;
    int stack_items_2 = 0;
    int stack_items = 0;
    StackData *stack_array = 0;
    StackData *new_stack_array = 0;
    byte *operator_array = 0;
    byte is_asm_function = 0;

    if(!(*operator_count) && has_return && !ternary_one_activated) {
        StackData return_data = PopScriptStackData(subject_script);
        stack_saves = malloc(sizeof(StackData));
        *stack_saves = return_data;
        save_count++;
        while(1) {
            return_data = PopScriptStackData(subject_script);
            if(return_data.type == OperationType) {
                (*operation_save) = *(uint32*) return_data.data;
                bytecode_2 = (byte) (((*operation_save) >> 8) & 255);
                if(bytecode_2 && bytecode_2 == StandardCallOpcode) {
                    StackData return_data = PopScriptStackData(subject_script);
                    if(return_data.error) return return_data.error;
                    standard_call_id = *(uint16*) return_data.data;
                }
                break;
            } else {
                if(return_data.error) return return_data.error;
                stack_saves = realloc(stack_saves, (save_count + 1) * sizeof(StackData));
                *(stack_saves + save_count) = return_data;
                save_count++;
            }
        }
        if((*operation_save) & HasFunctionCall) {
            StackData pointer_data = PopScriptStackData(subject_script);
            if(pointer_data.type == CFunctionType) is_asm_function = 1;
            *global_return_address = GetPointerStackData(pointer_data);
            parameter_count = 1;
        }
        for(int i = save_count; i > 0; i--) {
            if(error_code = PushScriptStackData(subject_script, (*(stack_saves + (i - 1))).data, (*(stack_saves + (i - 1))).type)) return error_code;
        }
        free(stack_saves);
        if(*operation_save & HasFunctionCall && is_asm_function) {
            if(error_code = PushScriptStackPointerNumber(subject_script, *global_return_address, CFunctionType)) return error_code;
        } else if((*operation_save) & HasFunctionCall) {
            if(error_code = PushScriptStackData(subject_script, (byte*) &parameter_count, UnsignedInt16Type)) return error_code;
            if(error_code = PushScriptStackPointer(subject_script, *global_return_address)) return error_code;
        } else if(bytecode_2 && bytecode_2 == StandardCallOpcode) {
            if(error_code = PushScriptStackData(subject_script, (byte*) &standard_call_id, UnsignedInt16Type)) return error_code;
        }

        subject_script->system.argument_count = 1;
        return ExecuteMegaScriptOperation(script_tasks, subject_script, (*operation_save));
    }

    while((*operator_count)) {
        int operator_items = 0; //marker: this may be needed in the future
        stack_items = 1;
        stack_array = malloc(sizeof(StackData));
        operator_array = malloc(1);
        byte inverted_previous = 255;
        OperatorInformation *inverted_previous_info = script_tasks->operator_info + inverted_previous; //Do the prefix and suffix manipulation first, associativity second.
        int index_temp = 0;
        int stack_items_temp = 0;

        if(error_code = CalculateOperationWithPrecedence(script_tasks, subject_script, operator_count, &inverted_previous, &inverted_previous_info, &stack_array, &stack_items, &operator_array, &operator_items, 1)) return error_code;

        if(first_iteration && has_return && !ternary_one_activated) {
            first_iteration = 0;
            StackData return_data = PopScriptStackData(subject_script);
            if(return_data.type == OperationType) {
                *operation_save = *(uint32*) return_data.data;
                if(*operation_save & HasFunctionCall) {
                    StackData return_address = PopScriptStackData(subject_script);
                    if(return_address.type == CFunctionType) is_asm_function = 1;
                    *global_return_address = GetPointerStackData(return_address);
                }
                bytecode_2 = (byte) (((*operation_save) >> 8) & 255);
                if(bytecode_2 == StandardCallOpcode) {
                    StackData return_data = PopScriptStackData(subject_script);
                    if(return_data.error) return return_data.error;
                    standard_call_id = *(uint16*) return_data.data;
                }
            } else {
                return MissingOperationSaveError;
            }
        }

        stack_items_2 = stack_items;
        parameter_count = stack_items;
        byte previous = 255;
        OperatorInformation *previous_info = script_tasks->operator_info + previous;

        StackData temp_sd = *(stack_array + (stack_items_2 - 1));
        PushScriptStackData(subject_script, temp_sd.data, temp_sd.type);
        
        int data_index = stack_items_2 - 1;
        
        for(int i = operator_items; i > 0; i--) {
            byte operator = *(operator_array + (i - 1));
            StackData *second_sd = (stack_array + (data_index - 1));
            if(error_code = SwitchScriptOperator(script_tasks, subject_script, operator, &previous, second_sd, operator_count, &data_index, &parameter_count, &stack_items_2, LeftToRight)) {
                free(stack_array);
                free(operator_array);
                return error_code;
            }
            previous_info = script_tasks->operator_info + previous;
        }

        if((*operator_count) > 0) if(error_code = FinalizeScriptOperators(script_tasks, subject_script, previous, &parameter_count, operator_count, &stack_items_2)) {
            free(stack_array);
            free(operator_array);
            return error_code;
        }
        
        stack_items = stack_items_2;
        free(stack_array);
        free(operator_array);
    }

    subject_script->system.argument_count = parameter_count;
    if(ternary_one_activated) {
        StackData bool_data = PopScriptStackData(subject_script);
        uint64 bool_value = CastToUnsignedInt64(bool_data);
        byte bool_byte = bool_value != 0;
        if(!bool_byte) if(error_code = SmartSkipStatement(script_tasks, subject_script, pc)) return error_code;
        return ExecuteMegaScriptOperation(script_tasks, subject_script, HasReturn);
    } else if(has_return) {
        if(*operation_save & HasFunctionCall && is_asm_function) {
            if(error_code = PushScriptStackPointerNumber(subject_script, *global_return_address, CFunctionType)) return error_code;
        } else if(*operation_save & HasFunctionCall) {
            if(error_code = PushScriptStackData(subject_script, (byte*) &parameter_count, UnsignedInt16Type)) return error_code;
            if(error_code = PushScriptStackPointer(subject_script, *global_return_address)) return error_code;
        } else if(bytecode_2 == StandardCallOpcode) {
            if(error_code = PushScriptStackData(subject_script, (byte*) &standard_call_id, UnsignedInt16Type)) return error_code;
        } else if(parameter_count > 1) return CommasCannotBeUsedHereError;
        return ExecuteMegaScriptOperation(script_tasks, subject_script, *operation_save);
    }
    return error_code;
}

int ExecuteMegaScriptOperation(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    uint32 old_operation_save = operation_save;
    uint64 *pc = &subject_script->values.program_counter; //Note: Order of function call variables are 16 bit parameter count first, 32/64 bit return pointer, and then 32 bit operation save last.
    byte resume_previous = (byte) (operation_save & ResumePrevious);
    byte has_return = (byte) (operation_save & HasReturn);
    byte finish_up = 0;
    byte last_operator = 255;
    int error_code = 0;
    unsigned short operator_count = (unsigned short) (operation_save >> 16);
    byte opcode = (byte) ((operation_save >> 8) & 255);
    uint64 global_return_address = 0;
    StackData return_value = {0};
    uint64 data_length = 0;
    byte ternary_one_activated = 0; //this second bytcode list is for when an operation is returned back to
    if(opcode) {
        uint64 program_counter = 0;
        byte bool_value;
        uint64 loop_head = 0;
        switch(opcode) {
            case PrintOpcode:
                error_code = StandardPrintCall(script_tasks, subject_script);
                break;
            case SizeofOpcode:
                error_code = StandardSizeofCall(script_tasks, subject_script);
                break;
            case SpaceofOpcode:
                error_code = StandardSpaceofCall(script_tasks, subject_script);
                break;
            case WaitOpcode:
                error_code = StandardWaitCall(script_tasks, subject_script);
                break;
            case LoadOpcode:
                error_code = StandardLoadCall(script_tasks, subject_script, StandardScriptType);
                break;
            case ReturnOpcode:
                return StandardReturnCall(script_tasks, subject_script);
            case ElseIfOpcode:
            case IfOpcode:
                return StandardIfCall(script_tasks, subject_script, operation_save);
            case WhileOpcode:
                return StandardWhileCall(script_tasks, subject_script, operation_save);
            case StandardCallOpcode:
                StackData standard_call_id_data = PopScriptStackData(subject_script);
                uint16 standard_call_id = *(uint16*) standard_call_id_data.data;
                error_code = StandardStandardCallCall(script_tasks, subject_script, standard_call_id);
                break;
            case JumpOpcode:
                return StandardJumpCall(script_tasks, subject_script);
        }
        if(error_code) return error_code;
    } else if(operation_save & HasFunctionCall) {
        StackData temp_function_data = CheckScriptStackData(subject_script);
        if(temp_function_data.type == CFunctionType) {
            PopScriptStackData(subject_script);
            int (*callback) (ScriptEnvironment*, Script*); //pointer marker
            callback = (void*) *(uint64*) temp_function_data.data;
            callback(script_tasks, subject_script);
        } else return PrepareFunctionCall(script_tasks, subject_script, operator_count, operation_save);
    }

    while(1) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        OperatorInformation *operator_info = script_tasks->operator_info + bytecode;
        byte prefix_looping = 1;
        byte *prefixes = 0;
        byte bracket_entered = 0;
        byte execute_bytecode = 1;

        if(!resume_previous && !finish_up) {
            while(prefix_looping) {
                bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
                byte is_loop = 0;
                switch(bytecode) {
                    case ElseOpcode:
                    case ElseIfOpcode:
                        if(error_code = SkipScopeData(script_tasks, subject_script, pc)) return error_code;
                    case StopOpcode:
                        return StandardStopCall(script_tasks, subject_script);
                    case ContinueOpcode:
                        return StandardContinueCall(script_tasks, subject_script);
                    case BreakOpcode:
                        return StandardBreakCall(script_tasks, subject_script, operation_save);
                    case CancelOpcode:
                        return StandardCancelCall(script_tasks, subject_script);
                    case ForOpcode:
                        return StandardForCall(script_tasks, subject_script, operation_save);
                    case 0:
                        return ZeroOperatorError;
                    case TernaryOneOperator:
                        ternary_one_activated = 1;
                    case StopStandardCallOpcode:
                        (*pc)++;
                    case EndStatementOpcode:
                        finish_up = 1;
                        prefix_looping = 0;
                        break;
                    case WhileOpcode:
                        if(error_code = PushScriptStackPointer(subject_script, *pc)) return error_code;
                    default:
                        OperatorInformation *operator_info = script_tasks->operator_info + bytecode;
                        switch(operator_info->bytecode_type) {
                            case StandardCallBytecodeType:
                                if(CheckStandardCallPrivilege(subject_script, bytecode)) return StandardCallPrivilegeError;
                                (*pc)++;
                                if(bytecode == StandardCallOpcode) {
                                    uint16 *standard_call_id = ReadScriptMemoryRange(script_tasks, subject_script, *pc, 2);
                                    if(standard_call_id) {
                                        *pc += 2;
                                        if(default_standard_calls[*standard_call_id].parameters == 0) {
                                            subject_script->system.argument_count = 0;
                                            if(error_code = StandardStandardCallCall(script_tasks, subject_script, *standard_call_id)) return error_code;
                                            bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
                                            if(bytecode == StopStandardCallOpcode) {
                                                (*pc)++;
                                            }
                                            execute_bytecode = 0;
                                            break;
                                        } else {
                                            if(error_code = PushScriptStackData(subject_script, (byte*) standard_call_id, UnsignedInt16Type)) return error_code;
                                        }
                                    } else {
                                        return StandardCallIdNotFoundError;
                                    }
                                }
                                operation_save = (operator_count << 16) | (bytecode << 8) | (operation_save & IsLoop) | ResumePrevious | has_return;
                                if(error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType)) return error_code;
                                return ExecuteMegaScriptOperation(script_tasks, subject_script, HasReturn);
                            case OperatorBytecodeType:
                                switch(operator_info->type) {
                                    case PrefixOperatorType:
                                        (*pc)++;
                                        operator_count++;
                                        error_code = PushScriptOperator(&subject_script->system, bytecode);
                                        if(error_code) return error_code;
                                        break;
                                    case OpeningBracketOperatorType:
                                        if(operation_save & IsFunctionArgument || operation_save & PrimitiveOnly) return ComplexOperationUsedError;
                                        (*pc)++;
                                        operation_save = (operator_count << 16) | (operation_save & IsLoop) | ResumePrevious | has_return;
                                        error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType);
                                        if(error_code) return error_code;
                                        return ExecuteMegaScriptOperation(script_tasks, subject_script, HasReturn);
                                }
                                if(operator_info->type == PrefixOperatorType) break;
                            case SyntaxBytecodeType:
                                prefix_looping = 0;
                        }
                }
            }

            
            if(execute_bytecode) {
                byte is_symbol = 0;
                if(ReadScriptMemory(script_tasks, subject_script, *pc) == VariableOpcode) is_symbol = 1; //maybe make this convert to symbol so you can run a function with a () after a pointer
                error_code = ExecuteMegaScriptOpcode(script_tasks, subject_script);
                if(error_code) return error_code;
                bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
                if(is_symbol && bytecode == CallOperator) {
                    if(operation_save & IsFunctionArgument || operation_save & PrimitiveOnly) return ComplexOperationUsedError;
                    (*pc)++;
                    if(ReadScriptMemory(script_tasks, subject_script, *pc) == CallEndOperator) {
                        (*pc)++;
                        StackData function_symbol = PopScriptStackData(subject_script);
                        if(function_symbol.error) return function_symbol.error;
                        function_symbol = ExpandStackData(script_tasks, subject_script, function_symbol);
                        if(function_symbol.error) return function_symbol.error;
                        if(function_symbol.type == CFunctionType) {
                            subject_script->system.argument_count = 0;
                            int (*callback) (ScriptEnvironment*, Script*); //pointer marker
                            callback = (void*) *(uint64*) function_symbol.data;
                            callback(script_tasks, subject_script);
                        } else if(function_symbol.type == PointerType) {
                            operation_save = (operator_count << 16) | (operation_save & IsLoop) | IsCallReturn | ResumePrevious | has_return;
                            return CallMegaScriptFunction(script_tasks, subject_script, 0, GetPointerStackData(function_symbol), operation_save);
                        } else return FunctionCallValuesScrambledError;
                    } else {
                        StackData symbol_data = PopScriptStackData(subject_script);
                        uint64 address = GetPointerStackData(symbol_data);
                        address = FindSymbolById(subject_script, address);
                        Symbol current_symbol = *(subject_script->values.symbol_table + address);
                        if(current_symbol.type == CFunctionType) {
                            byte *cfunction_data = ReadScriptMemoryRange(script_tasks, subject_script, current_symbol.address, bit_mode / 8); //pointer marker
                            if(!cfunction_data) return CouldNotReadMemoryError;
                            if(error_code = PushScriptStackData(subject_script, cfunction_data, CFunctionType)) return error_code;
                        } else {
                            byte *pointer = ReadScriptMemoryRange(script_tasks, subject_script, current_symbol.address, current_symbol.size);
                            error_code = PushScriptStackData(subject_script, pointer, PointerType);
                            if(error_code) return error_code;
                        }
                        operation_save = (operator_count << 16) | (operation_save & IsLoop) | HasFunctionCall | ResumePrevious | has_return;
                        error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType);
                        if(error_code) return error_code;
                        return ExecuteMegaScriptOperation(script_tasks, subject_script, HasReturn);
                    }
                }
                if(error_code) return error_code;
            }
        }

        if(!finish_up) {
            bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
            operator_info = script_tasks->operator_info + bytecode;

            //this doesn't work. in the future, try making it process things on the stack while they're being pushed
            /*if(operator_count > 1) {
                int operator_items = 0; //marker: this may be needed in the future
                int stack_items = 1;
                StackData *stack_array = malloc(sizeof(StackData));
                byte *operator_array = malloc(1);
                byte inverted_previous = 255;
                OperatorInformation *inverted_previous_info = script_tasks->operator_info + inverted_previous; //Do the prefix and suffix manipulation first, associativity second.
                int index_temp = 0;
                int stack_items_temp = 0;
                error_code = CalculateOperationWithPrecedence(script_tasks, subject_script, &operator_count, &inverted_previous, &inverted_previous_info, &stack_array, &stack_items, &operator_array, &operator_items, 0);

                if(error_code) {
                    free(stack_array);
                    free(operator_array);
                    return error_code;
                }

                int stack_items_2 = stack_items;
                uint16 parameter_count = stack_items;
                byte previous = 255;
                OperatorInformation *previous_info = script_tasks->operator_info + previous;

                StackData temp_sd = *(stack_array + (stack_items_2 - 1));
                PushScriptStackData(subject_script, temp_sd.data, temp_sd.type);

                int data_index = stack_items_2 - 1;

                for(int i = operator_items; i > 0; i--) {
                    byte operator = *(operator_array + (i - 1));
                    StackData *second_sd = (stack_array + (data_index - 1));
                    if(error_code = SwitchScriptOperator(script_tasks, subject_script, operator, &previous, second_sd, &operator_count, &data_index, &parameter_count, &stack_items_2, LeftToRight)) {
                        free(stack_array);
                        free(operator_array);
                        return error_code;
                    }
                    previous_info = script_tasks->operator_info + previous;
                }

                error_code = PushScriptOperator(&subject_script->system, previous);

                free(stack_array);
                free(operator_array);
                if(error_code) return error_code;
            }*/

            switch(operator_info->type) {
                case SuffixOperatorType:
                    error_code = PushScriptOperator(&subject_script->system, bytecode);
                    operator_count++;
                    if(error_code) return error_code;
                    (*pc)++;
                    bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
                    operator_info = script_tasks->operator_info + bytecode;
            }
             
            switch(bytecode) {
                case TernaryTwoOperator:
                    (*pc)++;
                    if(error_code = SmartSkipStatement(script_tasks, subject_script, pc)) return error_code;
                    finish_up = 1;
                    break;
                case TernaryOneOperator:
                    ternary_one_activated = 1;
                case StopStandardCallOpcode:
                    finish_up = 1;
                    (*pc)++;
                    break;
                case CommaOperator:
                    if(operation_save & StopAtComma || operation_save & IsFunctionArgument) {
                        if(!(operation_save & StopAtComma)) (*pc)++;
                    } else {
                        if(error_code = AddOperatorToArray(subject_script, operator_info, bytecode, &operator_count, pc, &finish_up, operation_save)) return error_code;
                        break;
                    }
                case EndStatementOpcode:
                    finish_up = 1;
                    break;
                default:
                    if(error_code = AddOperatorToArray(subject_script, operator_info, bytecode, &operator_count, pc, &finish_up, operation_save)) return error_code;
            }
        } else {
            return CalculateStackOperations(script_tasks, subject_script, &operator_count, &operation_save, has_return, ternary_one_activated, &global_return_address);
        }

        resume_previous = 0;
    }
    return 0;
}

int ExecuteMegaScriptStatement(ScriptEnvironment *script_tasks, Script *subject_script) {
    uint64 *pc = &subject_script->values.program_counter; //it is reversed on the stack. the data type is first, that way it can be popped off to see the data type
    while(1) {
        byte bytecode = ReadScriptMemory(script_tasks, subject_script, *pc);
        int error_code = 0;
        switch(bytecode) {
            case StartScriptOpcode:
                (*pc)++;
                break;
            case EndScriptOpcode:
                subject_script->values.state = FinishedState;
                if(CheckSetting(DeveloperNotifications)) printf("Script %d's state was set to Finished.\n", subject_script->id);
            case EndStatementOpcode:
                (*pc)++;
                return 0;
            default:
                error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, 0);
                if(error_code) return error_code;
                StackData temp_data = PopScriptStackData(subject_script);
                byte looping = 1;
                while(looping) {
                    looping = 0;
                    switch(temp_data.type) {
                        case TemporarySymbolType:
                            int64 debris_id = GetPointerStackData(temp_data);
                            int64 debris_index = FindDebris(subject_script, debris_id);
                            if(debris_index != -1) if(error_code = DeleteDebris(script_tasks, subject_script, debris_index)) return error_code;
                            break;
                        case AutoDeleteType:
                            if(CheckScriptStackData(subject_script).type == AutoDeleteType) {
                                temp_data = PopScriptStackData(subject_script);
                                looping = 1;
                            }
                    }
                }
        }
    }
    return 0;
}

int ExecuteMegaScriptCycle(ScriptEnvironment *script_tasks, Script *subject_script) {
    double current_time_difference = GetCurrentOSTime() - subject_script->values.started_at;
    if((0.0 < script_tasks->master.rules.script_timeout && current_time_difference > script_tasks->master.rules.script_timeout) || (0.0 < subject_script->rules.script_timeout && current_time_difference > subject_script->rules.script_timeout)) {
        return ScriptTimeoutError;
    }
    return ExecuteMegaScript(script_tasks, subject_script);
}

//make a new function called ExecuteMegaScriptCycle or ExecuteEnvironmentCycle
extern int ExecuteMegaScripts(ScriptEnvironment *script_tasks) {
    while(1) {
        short scripts_finished = 1;
        for(int i = 0; i < script_tasks->script_count; i++) {
            int error_code = 0;
            Script *subject_script = *(script_tasks->scripts + i);
            switch(subject_script->values.state) {
                case StandbyState:
                case PausedState:
                case DisabledState:
                    break;
                case FinishedState:
                    CheckMegaScriptEvents(script_tasks, subject_script);
                    break;
                case WaitingState:
                    double current_os_time = GetCurrentOSTime();
                    if(subject_script->values.waiting_until < current_os_time) {
                        subject_script->values.state = RunningState;
                        subject_script->values.started_at = current_os_time;
                    } else {
                        break;
                    }
                case RunningState:
                    switch(subject_script->type) {
                        case StandardScriptType:
                            if(!(error_code = ExecuteMegaScriptCycle(script_tasks, subject_script))) scripts_finished = 0;
                            break;
                    }
            }
            if(error_code) {
                PrintScriptError(script_tasks, subject_script, error_code, ExecutionErrorMode);
                ResetMegaScript(subject_script);
            }
        }
        if(scripts_finished) break;
    }
    return 0;
}

int InitializeLibrary(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    while(1) {
        short scripts_finished = 1;
        switch(subject_script->values.state) {
            case StandbyState:
            case PausedState:
            case DisabledState:
            case FinishedState:
                break;
            case WaitingState:
                subject_script->values.state = RunningState;
            case RunningState:
                if(!(error_code = ExecuteMegaScriptCycle(script_tasks, subject_script))) scripts_finished = 0;
        }
        if(error_code) {
            PrintScriptError(script_tasks, subject_script, error_code, ExecutionErrorMode);
            ResetMegaScript(subject_script);
            break;
        }
        if(scripts_finished) break;
    }
    return 0;
}

#define SkipWaitsToFullyInitialize 1U

int InitializeEnvironmentWithOptions(ScriptEnvironment *script_tasks, int options) {
    int error_code = 0;
    Script *subject_script = &script_tasks->master;
    while(1) {
        short scripts_finished = 1;
        switch(subject_script->values.state) {
            case StandbyState:
            case PausedState:
            case DisabledState:
            case FinishedState:
                break;
            case WaitingState:
                double current_os_time = GetCurrentOSTime();
                if(subject_script->values.waiting_until < current_os_time) {
                    subject_script->values.state = RunningState;
                    subject_script->values.started_at = current_os_time;
                } else {
                    if(options & SkipWaitsToFullyInitialize) scripts_finished = 0;
                    break;
                }
            case RunningState:
                if(!(error_code = ExecuteMegaScriptCycle(script_tasks, subject_script))) scripts_finished = 0;
        }
        if(error_code) {
            PrintScriptError(script_tasks, subject_script, error_code, ExecutionErrorMode);
            ResetMegaScript(subject_script);
            break;
        }
        if(scripts_finished) break;
    }
    return 0;
}

extern int InitializeEnvironment(ScriptEnvironment *script_tasks) {
    return InitializeEnvironmentWithOptions(script_tasks, SkipWaitsToFullyInitialize);
}

extern int ExecuteMasterCycle(ScriptEnvironment *script_tasks) {
    return InitializeEnvironmentWithOptions(script_tasks, 0);
}