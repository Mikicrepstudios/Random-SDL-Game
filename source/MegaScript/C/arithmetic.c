#define DestroyDebrisOnClose 1U

StackData ExpandStackStructInternal(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol, TemporarySymbol new_debris) {
    if(new_debris.type != StructType) return stack_symbol;
    StackData new_stack_data = stack_symbol;
    int multiply_factor = bit_mode / 8;
    new_stack_data.settings |= StackDataWasMallocated;
    if(new_debris.list_pointer) {
        int64 struct_size = GetRecursiveStructSize(script_tasks, subject_script, new_debris.list_pointer);
        if(struct_size == -1) {
            new_stack_data.error = FailedToGetStructSizeError;
            return new_stack_data;
        }
        new_stack_data.type = StructType;
        new_stack_data.data = malloc(multiply_factor + struct_size);
        memcpy(new_stack_data.data, &new_debris.list_pointer, multiply_factor);
        byte *struct_data = 0;
        if(new_debris.settings & HasGeniunePointer) {
            struct_data = (byte*) new_debris.address;
        } else {
            struct_data = ReadScriptMemoryRange(script_tasks, subject_script, new_debris.address, struct_size);
        }
        if(!struct_data) {
            new_stack_data.error = FailedToGetStructDataError;
            return new_stack_data;
        }
        memcpy(new_stack_data.data + multiply_factor, struct_data, struct_size);
        new_stack_data.length = struct_size;
    } else {
        new_stack_data.error = CouldNotFindStructListPointerError;
    }
    return new_stack_data;
}

StackData ExpandDebrisOptions(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol, byte options) {
    if(stack_symbol.type == TemporarySymbolType) {
        StackData ret_val = {0};
        uint64 data_address = GetPointerStackData(stack_symbol);
        int64 debris_index = FindDebris(subject_script, data_address);
        if(debris_index != -1) {
            TemporarySymbol *temp_symbol = subject_script->system.debris + debris_index;
            data_address = temp_symbol->address;
            if(temp_symbol->settings & IsCLike) {
                ret_val.length = GetDataTypeLength(temp_symbol->type);
                ret_val.type = temp_symbol->type;
                ret_val.sub_type = temp_symbol->sub_type;
                if(ret_val.type == ObjectType) {
                    ret_val.data = (byte*) &temp_symbol->address;
                } else if(temp_symbol->settings & HasGeniunePointer) {
                    ret_val.data = (byte*) temp_symbol->address;
                } else {
                    ret_val.data = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, ret_val.length);
                    if(!ret_val.data) ret_val.error = CouldNotReadMemoryError;
                    return ret_val;
                }
                if(options & DestroyDebrisOnClose) ret_val.error = DeleteDebris(script_tasks, subject_script, debris_index);
                return ret_val;
            } else {
                ret_val = GetStackDataFromMemory(script_tasks, subject_script, &data_address);
                if(options & DestroyDebrisOnClose) ret_val.error = DeleteDebris(script_tasks, subject_script, debris_index);
                return ret_val;
            }
        }
    }
    return stack_symbol;
}

StackData ExpandDebris(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    return ExpandDebrisOptions(script_tasks, subject_script, stack_symbol, 0);
}

StackData ExpandDebrisDelete(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    return ExpandDebrisOptions(script_tasks, subject_script, stack_symbol, DestroyDebrisOnClose);
}

StackData ExpandDataGeneric(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    Symbol *current = 0;
    switch(stack_symbol.type) {
        case SymbolType:
        case LabelType:
            current = GetSymbolFromStackData(subject_script, stack_symbol);
            if(current) {
                stack_symbol.type = current->type;
                stack_symbol.sub_type = current->sub_type;
                switch(stack_symbol.type) {
                    case StructType:
                        TemporarySymbol debris = {0};
                        debris.address = current->address;
                        debris.list_pointer = current->list_pointer;
                        debris.type = StructType;
                        stack_symbol = ExpandStackStructInternal(script_tasks, subject_script, stack_symbol, debris);
                        break;
                    case ObjectType:
                        stack_symbol.data = (byte*) &current->address;
                        stack_symbol.type = current->type;
                        stack_symbol.length = current->size;
                        break;
                    default:
                        if(stack_symbol.settings & HasGeniunePointer) {
                            stack_symbol.data = (byte*) current->address;
                        } else {
                            stack_symbol.data = script_tasks->memory + current->address; //marker: make it use the read function
                        }
                        stack_symbol.type = current->type;
                        stack_symbol.length = current->size;
                        stack_symbol.sub_type = current->sub_type;
                }
            }
            break;
        case Bool8ContainerType:
        case Bool16ContainerType:
        case Bool32ContainerType:
        case Bool64ContainerType:
            byte bool_index = *stack_symbol.data;
            byte *original_address = stack_symbol.data;
            byte bool_value = 0;
            stack_symbol.data++;
            if(*(stack_symbol.data + (bit_mode / 8)) & BoolHasGeniunePointer) stack_symbol.settings |= HasGeniunePointer;
            uint64 address = GetPointerFromStackData(stack_symbol);
            CalculationValues bool_values = {0};
            if(stack_symbol.settings & HasGeniunePointer) {
                stack_symbol.data = (byte*) address;
            } else {
                stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, address, GetDataTypeSize(stack_symbol.type));
            }
            if(!stack_symbol.data) {
                stack_symbol.error = CouldNotReadMemoryError;
                return stack_symbol;
            }
            switch(stack_symbol.type) {
                case Bool8ContainerType:
                    bool_value = (byte) ((*stack_symbol.data & (1 << bool_index)) != 0);
                    break;
                case Bool16ContainerType:
                    bool_value = (byte) ((*(uint16*) stack_symbol.data & ((uint16) 1 << bool_index)) != 0);
                    break;
                case Bool32ContainerType:
                    bool_value = (byte) ((*(uint32*) stack_symbol.data & ((uint32) 1 << bool_index)) != 0);
                    break;
                case Bool64ContainerType:
                    bool_value = (byte) ((*(uint64*) stack_symbol.data & ((uint64) 1 << bool_index)) != 0);
                    break;
            }
            *original_address = bool_value;
            stack_symbol.type = UnsignedInt8Type;
            stack_symbol.data = original_address;
    }
    return stack_symbol;
}

StackData FullyExpandStackDataOptions(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol, int options) {
    TemporarySymbol new_debris = GetDebrisFromStackDataOptions(subject_script, stack_symbol, 0);
    StackData new_stack_data = stack_symbol;
    uint64 *auxilary_pointer = 0;
    int multiply_factor = bit_mode / 8;
    switch(new_debris.type) {
        case 0:
            return stack_symbol;
        default:
            switch(new_debris.type) {
                case StructType:
                    new_stack_data.settings |= StackDataWasMallocated;
                    if(new_debris.list_pointer) {
                        int64 struct_size = GetRecursiveStructSize(script_tasks, subject_script, new_debris.list_pointer);
                        if(struct_size == -1) {
                            new_stack_data.error = FailedToGetStructSizeError;
                            return new_stack_data;
                        }
                        new_stack_data.type = StructType;
                        new_stack_data.data = malloc(multiply_factor + struct_size);
                        memcpy(new_stack_data.data, &new_debris.list_pointer, multiply_factor);
                        byte *struct_data = ReadScriptMemoryRange(script_tasks, subject_script, new_debris.address, struct_size);
                        if(!struct_data) {
                            new_stack_data.error = FailedToGetStructDataError;
                            return new_stack_data;
                        }
                        memcpy(new_stack_data.data + multiply_factor, struct_data, struct_size);
                        if(options & DeleteDebrisInStackData) {
                            int64 debris_index = FindDebris(subject_script, new_debris.id);
                            if(debris_index != -1) DeleteDebris(script_tasks, subject_script, debris_index);
                        }
                        new_stack_data.length = struct_size;
                        break;
                    } else {
                        new_stack_data.error = CouldNotFindStructListPointerError;
                    }
                    break;
                default:
                    if(options & DeleteDebrisInStackData) {
                        new_stack_data = ExpandDebrisDelete(script_tasks, subject_script, stack_symbol);
                        new_stack_data = ExpandDataGeneric(script_tasks, subject_script, stack_symbol);
                    } else {
                        new_stack_data = ExpandDebris(script_tasks, subject_script, stack_symbol);
                        new_stack_data = ExpandDataGeneric(script_tasks, subject_script, stack_symbol);
                    }
            }
    }
    return new_stack_data;
}

StackData ExpandStackDataNoDelete(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    stack_symbol = ExpandDebris(script_tasks, subject_script, stack_symbol);
    return ExpandDataGeneric(script_tasks, subject_script, stack_symbol);
}

StackData ExpandStackData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    stack_symbol = ExpandDebrisDelete(script_tasks, subject_script, stack_symbol);
    if(stack_symbol.error) return stack_symbol;
    return ExpandDataGeneric(script_tasks, subject_script, stack_symbol);
}

StackData ExpandStackSymbol(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    return ExpandDataGeneric(script_tasks, subject_script, stack_symbol);
}

StackData FullyExpandStackData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    return FullyExpandStackDataOptions(script_tasks, subject_script, stack_symbol, DeleteDebrisInStackData);
}

byte CastToUnsignedInt8(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (byte) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (byte) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (byte) (*(uint64*) input_symbol.data);
        case Int8Type:
            return *(byte*) input_symbol.data;
        case Int16Type:
            return (byte) (*(int16*) input_symbol.data);
        case Int32Type:
            return (byte) (*(int32*) input_symbol.data);
        case Int64Type:
            return (byte) (*(int64*) input_symbol.data);
        case FloatType:
            return (byte) (*(float32*) input_symbol.data);
        case DoubleType:
            return (byte) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (byte) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (byte) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

uint16 CastToUnsignedInt16(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (uint16) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return *(uint16*) input_symbol.data;
        case UnsignedInt32Type:
        case Bool32Type:
            return (uint16) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (uint16) (*(uint64*) input_symbol.data);
        case Int8Type:
            return (uint16) (*(char*) input_symbol.data);
        case Int16Type:
            return *(uint16*) input_symbol.data;
        case Int32Type:
            return (uint16) (*(int32*) input_symbol.data);
        case Int64Type:
            return (uint16) (*(int64*) input_symbol.data);
        case FloatType:
            return (uint16) (*(float32*) input_symbol.data);
        case DoubleType:
            return (uint16) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (uint16) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (uint16) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

uint32 CastToUnsignedInt32(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (uint32) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (uint32) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return *(uint32*) input_symbol.data;
        case UnsignedInt64Type:
        case Bool64Type:
            return (uint32) (*(uint64*) input_symbol.data);
        case Int8Type:
            return (uint32) (*(char*) input_symbol.data);
        case Int16Type:
            return (uint32) (*(int16*) input_symbol.data);
        case Int32Type:
            return *(uint32*) input_symbol.data;
        case Int64Type:
            return (uint32) (*(int64*) input_symbol.data);
        case FloatType:
            return (uint32) (*(float32*) input_symbol.data);
        case DoubleType:
            return (uint32) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (uint32) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (uint32) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

uint64 CastToUnsignedInt64(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (uint64) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (uint64) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (uint64) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return *(uint64*) input_symbol.data;
        case Int8Type:
            return (uint64) (*(char*) input_symbol.data);
        case Int16Type:
            return (uint64) (*(int16*) input_symbol.data);
        case Int32Type:
            return (uint64) (*(int32*) input_symbol.data);
        case Int64Type:
            return *(uint64*) input_symbol.data;
        case FloatType:
            return (uint64) (*(float32*) input_symbol.data);
        case DoubleType:
            return (uint64) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (uint64) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
        case ObjectType:
            return GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

char CastToInt8(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return *(char*) input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (char) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (char) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (char) (*(uint64*) input_symbol.data);
        case Int8Type:
            return *(char*) input_symbol.data;
        case Int16Type:
            return (char) (*(int16*) input_symbol.data);
        case Int32Type:
            return (char) (*(int32*) input_symbol.data);
        case Int64Type:
            return (char) (*(int64*) input_symbol.data);
        case FloatType:
            return (char) (*(float32*) input_symbol.data);
        case DoubleType:
            return (char) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (char) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (char) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

int16 CastToInt16(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (int16) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return *(int16*) input_symbol.data;
        case UnsignedInt32Type:
        case Bool32Type:
            return (int16) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (int16) (*(uint64*) input_symbol.data);
        case Int8Type:
            return (int16) (*(char*) input_symbol.data);
        case Int16Type:
            return *(int16*) input_symbol.data;
        case Int32Type:
            return (int16) (*(int32*) input_symbol.data);
        case Int64Type:
            return (int16) (*(int64*) input_symbol.data);
        case FloatType:
            return (int16) (*(float32*) input_symbol.data);
        case DoubleType:
            return (int16) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (int16) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (int16) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

int32 CastToInt32(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (int32) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (int32) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return *(int32*) input_symbol.data;
        case UnsignedInt64Type:
        case Bool64Type:
            return (int32) (*(uint64*) input_symbol.data);
        case Int8Type:
            return (int32) (*(char*) input_symbol.data);
        case Int16Type:
            return (int32) (*(int16*) input_symbol.data);
        case Int32Type:
            return *(int32*) input_symbol.data;
        case Int64Type:
            return (int32) (*(int64*) input_symbol.data);
        case FloatType:
            return (int32) (*(float32*) input_symbol.data);
        case DoubleType:
            return (int32) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (int32) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (int32) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

int64 CastToInt64(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (int64) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (int64) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (int64) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (int64) (*(uint64*) input_symbol.data);
        case Int8Type:
            return (int64) (*(char*) input_symbol.data);
        case Int16Type:
            return (int64) (*(int16*) input_symbol.data);
        case Int32Type:
            return (int64) (*(int32*) input_symbol.data);
        case Int64Type:
            return *(int64*) input_symbol.data;
        case FloatType:
            return (int64) (*(float32*) input_symbol.data);
        case DoubleType:
            return (int64) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (int64) (*(float128*) input_symbol.data);
        case CPointerType:
        case PointerType:
            return (int64) GetPointerStackData(input_symbol);
        default:
            return 0;
    }
}

float32 CastToFloat32(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (float32) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (float32) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (float32) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (float32) (*(int64*) input_symbol.data);
        case Int8Type:
            return (float32) (*(char*) input_symbol.data);
        case Int16Type:
            return (float32) (*(int16*) input_symbol.data);
        case Int32Type:
            return (float32) (*(int32*) input_symbol.data);
        case Int64Type:
            return (float32) (*(int64*) input_symbol.data);
        case FloatType:
            return *(float32*) input_symbol.data;
        case DoubleType:
            return (float32) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return (float32) (*(float128*) input_symbol.data);
        default:
            return 0.0f;
    }
}

float64 CastToFloat64(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (float64) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (float64) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (float64) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (float64) (*(int64*) input_symbol.data);
        case Int8Type:
            return (float64) (*(char*) input_symbol.data);
        case Int16Type:
            return (float64) (*(int16*) input_symbol.data);
        case Int32Type:
            return (float64) (*(int32*) input_symbol.data);
        case Int64Type:
            return (float64) (*(int64*) input_symbol.data);
        case FloatType:
            return (float64) (*(float32*) input_symbol.data);
        case DoubleType:
            return *(float64*) input_symbol.data;
        case LongDoubleType:
            return (float64) (*(float128*) input_symbol.data);
        default:
            return 0.0f;
    }
}

float128 CastToFloat128(StackData input_symbol) {
    switch(input_symbol.type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
        case Bool8Type:
            return (float128) *input_symbol.data;
        case UnsignedInt16Type:
        case Bool16Type:
            return (float128) (*(uint16*) input_symbol.data);
        case UnsignedInt32Type:
        case Bool32Type:
            return (float128) (*(uint32*) input_symbol.data);
        case UnsignedInt64Type:
        case Bool64Type:
            return (float128) (*(int64*) input_symbol.data);
        case Int8Type:
            return (float128) (*(char*) input_symbol.data);
        case Int16Type:
            return (float128) (*(int16*) input_symbol.data);
        case Int32Type:
            return (float128) (*(int32*) input_symbol.data);
        case Int64Type:
            return (float128) (*(int64*) input_symbol.data);
        case FloatType:
            return (float128) (*(float32*) input_symbol.data);
        case DoubleType:
            return (float128) (*(float64*) input_symbol.data);
        case LongDoubleType:
            return *(float128*) input_symbol.data;
        default:
            return 0.0f;
    }
}

#define StopAtZeroDimension 1U

TemporarySymbol GetDebrisFromStackDataOptions(Script *subject_script, StackData stack_symbol, int options) {
    TemporarySymbol current_symbol = {0};
    uint64 address = CastToUnsignedInt64(stack_symbol);
    switch(stack_symbol.type) {
        case SymbolType:
            Symbol *this_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
            if(!this_symbol) {
                return current_symbol;
            }
            if(options & StopAtZeroDimension && this_symbol->dimension <= 0) {
                return current_symbol;
            }
            current_symbol.dimension = this_symbol->dimension - 1;
            current_symbol.type = this_symbol->type;
            current_symbol.sub_type = this_symbol->sub_type;
            current_symbol.size = this_symbol->size;
            current_symbol.dimension = this_symbol->dimension;
            current_symbol.array_dimension = this_symbol->array_dimension;
            current_symbol.list_pointer = this_symbol->list_pointer;
            current_symbol.address = this_symbol->address;
            break;
        case TemporarySymbolType:
            int64 debris_index = FindDebris(subject_script, address);
            if(debris_index == -1) {
                return current_symbol;
            }
            TemporarySymbol *this_debris = subject_script->system.debris + debris_index;
            if(options & StopAtZeroDimension && this_debris->dimension <= 0) {
                return current_symbol;
            }
            current_symbol.dimension = this_debris->dimension - 1;
            current_symbol.type = this_debris->type;
            current_symbol.sub_type = this_debris->sub_type;
            current_symbol.size = this_debris->size;
            current_symbol.dimension = this_debris->dimension;
            current_symbol.array_dimension = this_debris->array_dimension;
            current_symbol.list_pointer = this_debris->list_pointer;
            current_symbol.id = this_debris->id;
            current_symbol.address = this_debris->address;
    }
    return current_symbol;
}

TemporarySymbol GetDebrisFromStackData(Script *subject_script, StackData stack_symbol) {
    return GetDebrisFromStackDataOptions(subject_script, stack_symbol, StopAtZeroDimension);
}

int AddData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    TemporarySymbol debris_item = GetDebrisFromStackData(subject_script, stack_symbol_1);
    int pointer_data_size = GetDataTypeLength(debris_item.sub_type);

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);

    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data + CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data + CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data + CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data + CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data + CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data + CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data + CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data + CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_1.data + CastToFloat32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_1.data + CastToFloat64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_1.data + CastToFloat128(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) + CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case CPointerType:
            debris_item.settings |= IsCLike;
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) + (CastToUnsignedInt64(stack_symbol_0) * pointer_data_size);
            debris_item.address = AllocateScriptMemory(script_tasks, subject_script, 8); //pointer marker
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, debris_item.address, 8, &data_values.val1_64_u)) return error_code;
            int64 debris_index = CreateDebris(script_tasks, subject_script, &debris_item);
            error_code = PushScriptStackPointerNumber(subject_script, debris_item.id, TemporarySymbolType);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int SubtractData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    TemporarySymbol debris_item = GetDebrisFromStackData(subject_script, stack_symbol_1);
    int pointer_data_size = GetDataTypeLength(debris_item.sub_type);
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);

    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data - CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data - CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data - CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data - CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data - CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data - CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data - CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data - CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_1.data - CastToFloat32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_1.data - CastToFloat64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_1.data - CastToFloat128(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) - CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case CPointerType:
            debris_item.settings |= IsCLike;
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) - (CastToUnsignedInt64(stack_symbol_0) * pointer_data_size);
            debris_item.address = AllocateScriptMemory(script_tasks, subject_script, 8); //pointer marker
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, debris_item.address, 8, &data_values.val1_64_u)) return error_code;
            int64 debris_index = CreateDebris(script_tasks, subject_script, &debris_item);
            error_code = PushScriptStackPointerNumber(subject_script, debris_item.id, TemporarySymbolType);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int MultiplyData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);

    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data + CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data * CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data * CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data * CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data * CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data * CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data * CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data * CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_1.data * CastToFloat32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_1.data * CastToFloat64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_1.data * CastToFloat128(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) * CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int DivideData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data / CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data / CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data / CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data / CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data / CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data / CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data / CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data / CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_1.data / CastToFloat32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_1.data / CastToFloat64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_1.data / CastToFloat128(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) / CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int RemainderOfData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data % CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data % CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data % CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data % CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data % CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data % CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data % CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data % CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
        case DoubleType:
        case LongDoubleType:
            return FloatingPointsModulatedError;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) % CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int ShiftDataLeft(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = (*(uint32*) stack_symbol_1.data << CastToInt32(stack_symbol_0));
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = (*(uint64*) stack_symbol_1.data << CastToInt32(stack_symbol_0));
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) << CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int ShiftDataRight(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = (*(uint32*) stack_symbol_1.data >> CastToInt32(stack_symbol_0));
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = (*(uint64*) stack_symbol_1.data >> CastToInt32(stack_symbol_0));
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) >> CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int BitwiseAndData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data & CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data & CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data & CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data & CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data & CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data & CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data & CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data & CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = (*(uint32*) stack_symbol_1.data & CastToUnsignedInt32(stack_symbol_0));
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = (*(uint64*) stack_symbol_1.data & CastToUnsignedInt64(stack_symbol_0));
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) & CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int BitwiseOrData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data | CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data | CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data | CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data | CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data | CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data | CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data | CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data | CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = (*(uint32*) stack_symbol_1.data | CastToUnsignedInt32(stack_symbol_0));
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = (*(uint64*) stack_symbol_1.data | CastToUnsignedInt64(stack_symbol_0));
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) | CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int BitwiseXorData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data ^ CastToUnsignedInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_1.data ^ CastToUnsignedInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_1.data ^ CastToUnsignedInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_1.data ^ CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_1.data ^ CastToInt8(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_1.data ^ CastToInt16(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_1.data ^ CastToInt32(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_1.data ^ CastToInt64(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = (*(uint32*) stack_symbol_1.data ^ CastToUnsignedInt32(stack_symbol_0));
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = (*(uint64*) stack_symbol_1.data ^ CastToUnsignedInt64(stack_symbol_0));
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_1) ^ CastToUnsignedInt64(stack_symbol_0);
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int BitwiseNotData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);

    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0); //you should make these give 1 (maybe)
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_0.data;
            data_values.val1_8_u = ~data_values.val1_8_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_0.data;
            data_values.val1_16_u = ~data_values.val1_16_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_0.data;
            data_values.val1_32_u = ~data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_0.data;
            data_values.val1_64_u = ~data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_0.data;
            data_values.val1_8 = ~data_values.val1_8;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_0.data;
            data_values.val1_16 = ~data_values.val1_16;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_0.data;
            data_values.val1_32 = ~data_values.val1_32;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_0.data;
            data_values.val1_64 = ~data_values.val1_64;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.val1_32_u = *(uint32*) stack_symbol_0.data;
            data_values.val1_32_u = ~data_values.val1_32_u;
            data_values.f1_32 = *(float32*) &data_values.val1_32_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.val1_64_u = *(uint64*) stack_symbol_0.data;
            data_values.val1_64_u = ~data_values.val1_64_u;
            data_values.f1_64 = *(float64*) &data_values.val1_64_u;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case PointerType:
        case CPointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_0);
            data_values.val1_64_u = ~data_values.val1_64_u;
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        case LongDoubleType:
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int HandleAssignData(ScriptEnvironment *script_tasks, Script *subject_script, byte data_type, uint64 address, uint64 auxilary, uint32 size, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(data_type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_8_u)) return error_code;
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_16_u)) return error_code;
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_32_u)) return error_code;
            break;
        case UnsignedInt64Type:
        case CPointerType:
        case PointerType:
            data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0); //pointer marker
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_64_u)) return error_code;
            break;
        case ObjectType:
            if(stack_symbol_0.type != ObjectType) return InvalidOperationError;
            data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0); //pointer marker
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_64_u)) return error_code;
            break;
        case Int8Type:
            data_values.val1_8 = CastToInt8(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_8)) return error_code;
            break;
        case Int16Type:
            data_values.val1_16 = CastToInt16(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_16)) return error_code;
            break;
        case Int32Type:
            data_values.val1_32 = CastToInt32(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_32)) return error_code;
            break;
        case Int64Type:
            data_values.val1_64 = CastToInt64(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.val1_64)) return error_code;
            break;
        case FloatType:
            data_values.f1_32 = CastToFloat32(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.f1_32)) return error_code;
            break;
        case DoubleType:
            data_values.f1_64 = CastToFloat64(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.f1_64)) return error_code;
            break;
        case LongDoubleType:
            data_values.f1_128 = CastToFloat128(stack_symbol_0);
            if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, size, &data_values.f1_128)) return error_code;
            break;
        case StructType:
            if(stack_symbol_0.type != StructType) return AttemptToAssignNonStructToStructError;
            uint64 list_pointer = *(uint64*) stack_symbol_0.data;
            stack_symbol_0.data += (bit_mode / 8);
            if(auxilary == list_pointer) {
                if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, address, stack_symbol_0.length, stack_symbol_0.data)) return error_code;
            } else return ListPointerDoesNotMatchError;
            break;
        default:
            return InvalidOperationError;
    }
    return 0;
}

int HandleAssignRealData(ScriptEnvironment *script_tasks, Script *subject_script, byte data_type, uint64 address, uint64 auxilary, uint32 size, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    byte *real_address = (byte*) address; //pointer marker
    switch(data_type) {
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            *real_address = CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            *((uint16*) real_address) = CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            *((uint32*) real_address) = CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
        case CPointerType:
        case PointerType:
            *((uint64*) real_address) = CastToUnsignedInt64(stack_symbol_0); //pointer marker
            break;
        case ObjectType:
            if(stack_symbol_0.type != ObjectType) return InvalidOperationError;
            *((uint64*) real_address) = CastToUnsignedInt64(stack_symbol_0); //pointer marker
            break;
        case Int8Type:
            *((char*) real_address) = CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            *((int16*) real_address) = CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            *((int32*) real_address) = CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            *((int64*) real_address) = CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            *((float32*) real_address) = CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            *((float64*) real_address) = CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            *((float128*) real_address) = CastToFloat128(stack_symbol_0);
            break;
        case StructType:
            if(stack_symbol_0.type != StructType) return AttemptToAssignNonStructToStructError;
            uint64 list_pointer = *(uint64*) stack_symbol_0.data;
            stack_symbol_0.data += (bit_mode / 8);
            if(auxilary == list_pointer) {
                memcpy((byte*) address, stack_symbol_0.data, stack_symbol_0.length);
            } else return ListPointerDoesNotMatchError;
            break;
        default:
            return InvalidOperationError;
    }
    return 0;
}

int AssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    StackData original_symbol = stack_symbol_1;
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case TemporarySymbolType:
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            uint64 debris_id = GetPointerStackData(stack_symbol_1); //pointer marker
            int64 debris_index = FindDebris(subject_script, debris_id);
            if(debris_index == -1) return DebrisNotFoundError;
            TemporarySymbol *debris_item = subject_script->system.debris + debris_index;
            if(debris_item->settings & IsCLike) {
                if(debris_item->settings & HasGeniunePointer) {
                    if(error_code = HandleAssignRealData(script_tasks, subject_script, debris_item->type, debris_item->address, debris_item->list_pointer, debris_item->size, stack_symbol_0)) return error_code;
                } else {
                    if(error_code = HandleAssignData(script_tasks, subject_script, debris_item->type, debris_item->address, debris_item->list_pointer, debris_item->size, stack_symbol_0)) return error_code;
                }
            } else {
                if(error_code = WriteStackDataToMemory(script_tasks, subject_script, debris_item->address, stack_symbol_0)) return error_code; //make sure to push here in the future
            }
            return PushScriptStackData(subject_script, 0, 0); //fix this
        case Bool8ContainerType:
        case Bool16ContainerType:
        case Bool32ContainerType:
        case Bool64ContainerType:
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
            data_values.val1_8_u = (byte) (data_values.val1_64_u != 0);
            byte bool_index = *stack_symbol_1.data;
            stack_symbol_1.data++;
            uint64 bool_pointer = GetPointerFromStackData(stack_symbol_1);
            if(*(stack_symbol_1.data + (bit_mode / 8)) & BoolHasGeniunePointer) stack_symbol_1.settings |= HasGeniunePointer;
            if(stack_symbol_1.settings & HasGeniunePointer) {
                stack_symbol_1.data = (byte*) bool_pointer;
            } else {
                stack_symbol_1.data = ReadScriptMemoryRange(script_tasks, subject_script, bool_pointer, GetDataTypeSize(stack_symbol_1.type));
            }
            if(!stack_symbol_1.data) return BoolPointerError;

            uint64 bool_value = 0;
            switch(stack_symbol_1.type) {
                case Bool8ContainerType:
                    bool_value = (uint64) *stack_symbol_1.data;
                    break;
                case Bool16ContainerType:
                    bool_value = (uint64) *(uint16*) stack_symbol_1.data;
                    break;
                case Bool32ContainerType:
                    bool_value = (uint64) *(uint32*) stack_symbol_1.data;
                    break;
                case Bool64ContainerType:
                    bool_value = *(uint64*) stack_symbol_1.data;
            }
            if(data_values.val1_8_u) {
                bool_value |= 1 << bool_index; //set to true
            } else {
                bool_value &= ~(1 << bool_index); //set to false
            }
            switch(stack_symbol_1.type) {
                case Bool8ContainerType:
                    *stack_symbol_1.data = (byte) bool_value;
                    break;
                case Bool16ContainerType:
                    *(uint16*) stack_symbol_1.data = (uint16) bool_value;
                    break;
                case Bool32ContainerType:
                    *(uint32*) stack_symbol_1.data = (uint32) bool_value;
                    break;
                case Bool64ContainerType:
                    *(uint64*) stack_symbol_1.data = (uint64) bool_value;
            }
            return PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
        case SymbolType:
            uint64 address = GetPointerStackData(stack_symbol_1); //pointer marker
            address = FindSymbolById(subject_script, address);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            Symbol *second = 0;
            if(stack_symbol_0.type == SymbolType) {
                address = GetPointerStackData(stack_symbol_0);
                second = (subject_script->values.symbol_table + address);
                if(second->type == ObjectType) address = second->address;
            } else {
                stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
                address = CastToUnsignedInt64(stack_symbol_0);
                if(stack_symbol_0.type == ObjectType && !address) return ObjectDoesNotExistError;
            }
            switch(current->type) {
                case ObjectType:
                    if(second) if(second->type != ObjectType) return AttemptedToAssignNonObjectToObjectError;
                    if(!address) return ObjectDoesNotExistError;
                    current->address = address;
                    break;
                default:
                    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
                    if(current->settings & HasGeniunePointer) {
                        if(error_code = HandleAssignRealData(script_tasks, subject_script, stack_symbol_1.type, *(uint64*) stack_symbol_1.data, current->list_pointer, stack_symbol_1.length, stack_symbol_0)) return error_code; //pointer marker
                    } else {
                        if(!ReadScriptMemoryRange(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
                        stack_symbol_1.data = script_tasks->memory + current->address;
                        if(error_code = HandleAssignData(script_tasks, subject_script, stack_symbol_1.type, stack_symbol_1.data - script_tasks->memory, current->list_pointer, stack_symbol_1.length, stack_symbol_0)) return error_code;
                    }
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int AddAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.sub_type = current->sub_type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data + CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data + CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data + CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data + CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data + CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data + CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data + CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data + CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.f1_32 = *(float32*) stack_symbol_1.data + CastToFloat32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_32, stack_symbol_1.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = *(float64*) stack_symbol_1.data + CastToFloat64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_64, stack_symbol_1.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = *(float128*) stack_symbol_1.data + CastToFloat128(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_128, stack_symbol_1.length);
                    break;
                case CPointerType:
                    byte data_length = GetDataTypeLength(stack_symbol_1.sub_type);
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data + (CastToUnsignedInt64(stack_symbol_0) * data_length);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int SubtractAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.sub_type = current->sub_type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data - CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data - CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data - CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data - CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data - CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data - CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data - CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data - CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.f1_32 = *(float32*) stack_symbol_1.data - CastToFloat32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_32, stack_symbol_1.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = *(float64*) stack_symbol_1.data - CastToFloat64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_64, stack_symbol_1.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = *(float128*) stack_symbol_1.data - CastToFloat128(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_128, stack_symbol_1.length);
                    break;
                case CPointerType:
                    byte data_length = GetDataTypeLength(stack_symbol_1.sub_type);
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data - (CastToUnsignedInt64(stack_symbol_0) * data_length);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int MultiplyAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data * CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data * CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data * CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data * CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data * CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data * CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data * CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data * CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.f1_32 = *(float32*) stack_symbol_1.data * CastToFloat32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_32, stack_symbol_1.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = *(float64*) stack_symbol_1.data * CastToFloat64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_64, stack_symbol_1.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = *(float128*) stack_symbol_1.data * CastToFloat128(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_128, stack_symbol_1.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int DivideAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data / CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data / CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data / CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data / CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data / CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data / CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data / CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data / CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.f1_32 = *(float32*) stack_symbol_1.data / CastToFloat32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_32, stack_symbol_1.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = *(float64*) stack_symbol_1.data / CastToFloat64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_64, stack_symbol_1.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = *(float128*) stack_symbol_1.data / CastToFloat128(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.f1_128, stack_symbol_1.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int RemainderAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = (*stack_symbol_1.data) % CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data % CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data % CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data % CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data % CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data % CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data % CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data % CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int ShiftLeftAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data << CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data << CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data << CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data << CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data << CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data << CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data << CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data << CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.val1_32_u = (*(uint32*) stack_symbol_1.data << CastToInt32(stack_symbol_0));
                    data_values.f1_32 = *(float32*) &data_values.val1_32_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_32, 4);
                    break;
                case DoubleType:
                    data_values.val1_64_u = (*(uint64*) stack_symbol_1.data << CastToInt32(stack_symbol_0));
                    data_values.f1_64 = *(float64*) &data_values.val1_64_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_64, 8);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int ShiftRightAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data >> CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data >> CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data >> CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data >> CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data >> CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data >> CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data >> CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data >> CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.val1_32_u = (*(uint32*) stack_symbol_1.data >> CastToInt32(stack_symbol_0));
                    data_values.f1_32 = *(float32*) &data_values.val1_32_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_32, 4);
                    break;
                case DoubleType:
                    data_values.val1_64_u = (*(uint64*) stack_symbol_1.data >> CastToInt32(stack_symbol_0));
                    data_values.f1_64 = *(float64*) &data_values.val1_64_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_64, 8);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int AndAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data & CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data & CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data & CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data & CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data & CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data & CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data & CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data & CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.val1_32_u = (*(uint32*) stack_symbol_1.data & CastToInt32(stack_symbol_0));
                    data_values.f1_32 = *(float32*) &data_values.val1_32_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_32, 4);
                    break;
                case DoubleType:
                    data_values.val1_64_u = (*(uint64*) stack_symbol_1.data & CastToInt32(stack_symbol_0));
                    data_values.f1_64 = *(float64*) &data_values.val1_64_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_64, 8);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int XorAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data ^ CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data ^ CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data ^ CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data ^ CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data ^ CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data ^ CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data ^ CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data ^ CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.val1_32_u = (*(uint32*) stack_symbol_1.data ^ CastToInt32(stack_symbol_0));
                    data_values.f1_32 = *(float32*) &data_values.val1_32_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_32, 4);
                    break;
                case DoubleType:
                    data_values.val1_64_u = (*(uint64*) stack_symbol_1.data ^ CastToInt32(stack_symbol_0));
                    data_values.f1_64 = *(float64*) &data_values.val1_64_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_64, 8);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int OrAssignData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_1.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_1;
            uint64 address = GetPointerStackData(stack_symbol_1);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_1.type = current->type;
            stack_symbol_1.length = GetDataTypeLength(stack_symbol_1.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_1.length)) return CouldNotWriteMemoryError;
            stack_symbol_1.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_1.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = *stack_symbol_1.data | CastToUnsignedInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8_u, stack_symbol_1.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = *(uint16*) stack_symbol_1.data | CastToUnsignedInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16_u, stack_symbol_1.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = *(uint32*) stack_symbol_1.data | CastToUnsignedInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32_u, stack_symbol_1.length);
                    break;
                case UnsignedInt64Type:
                case CPointerType:
                case PointerType:
                    data_values.val1_64_u = *(uint64*) stack_symbol_1.data | CastToUnsignedInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = *(char*) stack_symbol_1.data | CastToInt8(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_8, stack_symbol_1.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = *(int16*) stack_symbol_1.data | CastToInt16(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_16, stack_symbol_1.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = *(int32*) stack_symbol_1.data | CastToInt32(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_32, stack_symbol_1.length);
                    break;
                case Int64Type:
                    data_values.val1_64_u = *(int64*) stack_symbol_1.data | CastToInt64(stack_symbol_0);
                    memcpy(stack_symbol_1.data, &data_values.val1_64_u, stack_symbol_1.length);
                    break;
                case FloatType:
                    data_values.val1_32_u = (*(uint32*) stack_symbol_1.data | CastToInt32(stack_symbol_0));
                    data_values.f1_32 = *(float32*) &data_values.val1_32_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_32, 4);
                    break;
                case DoubleType:
                    data_values.val1_64_u = (*(uint64*) stack_symbol_1.data | CastToInt32(stack_symbol_0));
                    data_values.f1_64 = *(float64*) &data_values.val1_64_u;
                    memcpy(stack_symbol_1.data, &data_values.f1_64, 8);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return AssignedNonSymbolError;
    }
    return error_code;
}

int PrefixIncrementData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_0;
            uint64 address = GetPointerStackData(stack_symbol_0);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_0.type = current->type;
            stack_symbol_0.length = GetDataTypeLength(stack_symbol_0.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_0.length)) return CouldNotWriteMemoryError;
            stack_symbol_0.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_0.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    data_values.val1_8_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_8_u, stack_symbol_0.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
                    data_values.val1_16_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_16_u, stack_symbol_0.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
                    data_values.val1_32_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_32_u, stack_symbol_0.length);
                    break;
                case UnsignedInt64Type:
                case PointerType:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    data_values.val1_64_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = CastToInt8(stack_symbol_0);
                    data_values.val1_8++;
                    memcpy(stack_symbol_0.data, &data_values.val1_8, stack_symbol_0.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = CastToInt16(stack_symbol_0);
                    data_values.val1_16++;
                    memcpy(stack_symbol_0.data, &data_values.val1_16, stack_symbol_0.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = CastToInt32(stack_symbol_0);
                    data_values.val1_32++;
                    memcpy(stack_symbol_0.data, &data_values.val1_32, stack_symbol_0.length);
                    break;
                case Int64Type:
                    data_values.val1_64 = CastToInt64(stack_symbol_0);
                    data_values.val1_64++;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case FloatType:
                    data_values.f1_32 = CastToFloat32(stack_symbol_0);
                    data_values.f1_32++;
                    memcpy(stack_symbol_0.data, &data_values.f1_32, stack_symbol_0.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = CastToFloat64(stack_symbol_0);
                    data_values.f1_64++;
                    memcpy(stack_symbol_0.data, &data_values.f1_64, stack_symbol_0.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = CastToFloat128(stack_symbol_0);
                    data_values.f1_128++;
                    memcpy(stack_symbol_0.data, &data_values.f1_128, stack_symbol_0.length);
                    break;
                case CPointerType:
                    byte data_length = GetDataTypeLength(stack_symbol_0.sub_type);
                    data_values.val1_64_u = *(uint64*) stack_symbol_0.data + data_length; //pointer marker
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return IncrementedNonSymbolError;
    }
    return error_code;
}

int PrefixDecrementData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_0;
            uint64 address = GetPointerStackData(stack_symbol_0);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_0.type = current->type;
            stack_symbol_0.length = GetDataTypeLength(stack_symbol_0.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_0.length)) return CouldNotWriteMemoryError;
            stack_symbol_0.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_0.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    data_values.val1_8_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_8_u, stack_symbol_0.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
                    data_values.val1_16_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_16_u, stack_symbol_0.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
                    data_values.val1_32_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_32_u, stack_symbol_0.length);
                    break;
                case UnsignedInt64Type:
                case PointerType:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    data_values.val1_64_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = CastToInt8(stack_symbol_0);
                    data_values.val1_8--;
                    memcpy(stack_symbol_0.data, &data_values.val1_8, stack_symbol_0.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = CastToInt16(stack_symbol_0);
                    data_values.val1_16--;
                    memcpy(stack_symbol_0.data, &data_values.val1_16, stack_symbol_0.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = CastToInt32(stack_symbol_0);
                    data_values.val1_32--;
                    memcpy(stack_symbol_0.data, &data_values.val1_32, stack_symbol_0.length);
                    break;
                case Int64Type:
                    data_values.val1_64 = CastToInt64(stack_symbol_0);
                    data_values.val1_64--;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case FloatType:
                    data_values.f1_32 = CastToFloat32(stack_symbol_0);
                    data_values.f1_32--;
                    memcpy(stack_symbol_0.data, &data_values.f1_32, stack_symbol_0.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = CastToFloat64(stack_symbol_0);
                    data_values.f1_64--;
                    memcpy(stack_symbol_0.data, &data_values.f1_64, stack_symbol_0.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = CastToFloat128(stack_symbol_0);
                    data_values.f1_128--;
                    memcpy(stack_symbol_0.data, &data_values.f1_128, stack_symbol_0.length);
                    break;
                case CPointerType:
                    byte data_length = GetDataTypeLength(stack_symbol_0.sub_type);
                    data_values.val1_64_u = *(uint64*) stack_symbol_0.data - data_length; //pointer marker
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                default:
                    return InvalidOperationError;
            }
            error_code = PushScriptStackData(subject_script, original_symbol.data, original_symbol.type);
            break;
        default:
            return DecrementedNonSymbolError;
    }
    return error_code;
}

int IncrementData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_0;
            uint64 address = GetPointerStackData(stack_symbol_0);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_0.type = current->type;
            stack_symbol_0.length = GetDataTypeLength(stack_symbol_0.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_0.length)) return CouldNotWriteMemoryError;
            stack_symbol_0.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_0.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type)) return error_code;
                    data_values.val1_8_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_8_u, stack_symbol_0.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type)) return error_code;
                    data_values.val1_16_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_16_u, stack_symbol_0.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type)) return error_code;
                    data_values.val1_32_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_32_u, stack_symbol_0.length);
                    break;
                case UnsignedInt64Type:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type)) return error_code;
                    data_values.val1_64_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = CastToInt8(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type)) return error_code;
                    data_values.val1_8++;
                    memcpy(stack_symbol_0.data, &data_values.val1_8, stack_symbol_0.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = CastToInt16(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type)) return error_code;
                    data_values.val1_16++;
                    memcpy(stack_symbol_0.data, &data_values.val1_16, stack_symbol_0.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = CastToInt32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type)) return error_code;
                    data_values.val1_32++;
                    memcpy(stack_symbol_0.data, &data_values.val1_32, stack_symbol_0.length);
                    break;
                case Int64Type:
                    data_values.val1_64 = CastToInt64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type)) return error_code;
                    data_values.val1_64++;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case FloatType:
                    data_values.f1_32 = CastToFloat32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType)) return error_code;
                    data_values.f1_32++;
                    memcpy(stack_symbol_0.data, &data_values.f1_32, stack_symbol_0.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = CastToFloat64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType)) return error_code;
                    data_values.f1_64++;
                    memcpy(stack_symbol_0.data, &data_values.f1_64, stack_symbol_0.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = CastToFloat128(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType)) return error_code;
                    data_values.f1_128++;
                    memcpy(stack_symbol_0.data, &data_values.f1_128, stack_symbol_0.length);
                    break;
                case PointerType:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    if(error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u)) return error_code;
                    data_values.val1_64_u++;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case CPointerType:
                    TemporarySymbol debris_item = GetDebrisFromStackData(subject_script, original_symbol);
                    TemporarySymbol new_debris = {0};
                    int pointer_data_size = GetDataTypeLength(debris_item.sub_type);
                    if(debris_item.dimension > 1) pointer_data_size = GetDataTypeLength(debris_item.type);
                    new_debris.settings |= IsCLike;
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0) + pointer_data_size;
                    new_debris.address = AllocateScriptMemory(script_tasks, subject_script, 8); //pointer marker
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    int64 debris_index = CreateDebris(script_tasks, subject_script, &new_debris);
                    error_code = PushScriptStackPointerNumber(subject_script, new_debris.id, TemporarySymbolType);
                    break;
                default:
                    return InvalidOperationError;
            }
            break;
        default:
            return IncrementedNonSymbolError;
    }
    return error_code;
}

int DecrementData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case SymbolType:
            StackData original_symbol = stack_symbol_0;
            uint64 address = GetPointerStackData(stack_symbol_0);
            address = FindSymbolById(subject_script, address);
            stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
            Symbol *current = (subject_script->values.symbol_table + address);
            stack_symbol_0.type = current->type;
            stack_symbol_0.length = GetDataTypeLength(stack_symbol_0.type);
            if(!ReadScriptMemoryRangeCheckIfReadOnly(script_tasks, subject_script, current->address, stack_symbol_0.length)) return CouldNotWriteMemoryError;
            stack_symbol_0.data = script_tasks->memory + current->address;
            address = current->address;
            switch(stack_symbol_0.type) {
                case CharType:
                case DataType:
                case UnsignedInt8Type:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type)) return error_code;
                    data_values.val1_8_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_8_u, stack_symbol_0.length);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type)) return error_code;
                    data_values.val1_16_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_16_u, stack_symbol_0.length);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type)) return error_code;
                    data_values.val1_32_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_32_u, stack_symbol_0.length);
                    break;
                case UnsignedInt64Type:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type)) return error_code;
                    data_values.val1_64_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case Int8Type:
                    data_values.val1_8 = CastToInt8(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type)) return error_code;
                    data_values.val1_8--;
                    memcpy(stack_symbol_0.data, &data_values.val1_8, stack_symbol_0.length);
                    break;
                case Int16Type:
                    data_values.val1_16 = CastToInt16(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type)) return error_code;
                    data_values.val1_16--;
                    memcpy(stack_symbol_0.data, &data_values.val1_16, stack_symbol_0.length);
                    break;
                case Int32Type:
                    data_values.val1_32 = CastToInt32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type)) return error_code;
                    data_values.val1_32--;
                    memcpy(stack_symbol_0.data, &data_values.val1_32, stack_symbol_0.length);
                    break;
                case Int64Type:
                    data_values.val1_64 = CastToInt64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type)) return error_code;
                    data_values.val1_64--;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case FloatType:
                    data_values.f1_32 = CastToFloat32(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType)) return error_code;
                    data_values.f1_32--;
                    memcpy(stack_symbol_0.data, &data_values.f1_32, stack_symbol_0.length);
                    break;
                case DoubleType:
                    data_values.f1_64 = CastToFloat64(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType)) return error_code;
                    data_values.f1_64--;
                    memcpy(stack_symbol_0.data, &data_values.f1_64, stack_symbol_0.length);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = CastToFloat128(stack_symbol_0);
                    if(error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType)) return error_code;
                    data_values.f1_128--;
                    memcpy(stack_symbol_0.data, &data_values.f1_128, stack_symbol_0.length);
                    break;
                case PointerType:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    if(error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u)) return error_code;
                    data_values.val1_64_u--;
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    break;
                case CPointerType:
                    TemporarySymbol debris_item = GetDebrisFromStackData(subject_script, original_symbol);
                    TemporarySymbol new_debris = {0};
                    int pointer_data_size = GetDataTypeLength(debris_item.sub_type);
                    if(debris_item.dimension > 1) pointer_data_size = GetDataTypeLength(debris_item.type);
                    new_debris.settings |= IsCLike;
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0) - pointer_data_size;
                    new_debris.address = AllocateScriptMemory(script_tasks, subject_script, 8); //pointer marker
                    memcpy(stack_symbol_0.data, &data_values.val1_64_u, stack_symbol_0.length);
                    int64 debris_index = CreateDebris(script_tasks, subject_script, &new_debris);
                    error_code = PushScriptStackPointerNumber(subject_script, new_debris.id, TemporarySymbolType);
                    break;
                default:
                    return InvalidOperationError;
            }
            break;
        default:
            return DecrementedNonSymbolError;
    }
    return error_code;
}

int NegateData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_0.data * -1;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_0.data * -1.0;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_0.data * -1.0;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_0.data * -1.0;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_0) * -1;
            error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int PositateData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    
    switch(stack_symbol_0.type) {
        case 0:
            error_code = PushScriptStackData(subject_script, 0, 0);
            break;
        case NullType:
            error_code = PushScriptStackData(subject_script, 0, NullType);
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
            break;
        case UnsignedInt16Type:
            data_values.val1_16_u = *(uint16*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
            break;
        case UnsignedInt32Type:
            data_values.val1_32_u = *(uint32*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
            break;
        case UnsignedInt64Type:
            data_values.val1_64_u = *(uint64*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        case Int8Type:
            data_values.val1_8 = *(char*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
            break;
        case Int16Type:
            data_values.val1_16 = *(int16*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
            break;
        case Int32Type:
            data_values.val1_32 = *(int32*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
            break;
        case Int64Type:
            data_values.val1_64 = *(int64*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
            break;
        case FloatType:
            data_values.f1_32 = *(float32*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
            break;
        case DoubleType:
            data_values.f1_64 = *(float64*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
            break;
        case LongDoubleType:
            data_values.f1_128 = *(float128*) stack_symbol_0.data;
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
            break;
        case PointerType:
            data_values.val1_64_u = GetPointerStackData(stack_symbol_0);
            error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int AddressOfData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    int error_code = 0;
    CalculationValues data_values = {0};
    int multiply_factor = bit_mode / 8;

    switch(stack_symbol_0.type) {
        case SymbolType:
            Symbol *current_symbol = GetSymbolFromStackData(subject_script, stack_symbol_0);
            if(current_symbol) {
                if(!(script_tasks->settings.bools & EnvironmentUsesRealAddresses) && current_symbol->type == ObjectType) return AttemptedToGetAddressOfObjectError;
                if(current_symbol->settings & IsCLike) {
                    TemporarySymbol debris_item = {0};
                    debris_item.dimension = current_symbol->dimension + 1;
                    debris_item.array_dimension = current_symbol->array_dimension;
                    debris_item.settings |= IsCLike;
                    debris_item.type = current_symbol->type;
                    debris_item.sub_type = current_symbol->sub_type;
                    if(debris_item.type != CPointerType) {
                        debris_item.sub_type = debris_item.type;
                        debris_item.type = CPointerType;
                    }
                    debris_item.size = multiply_factor; //pointer marker
                    debris_item.address = AllocateScriptMemory(script_tasks, subject_script, multiply_factor);
                    if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, debris_item.address, multiply_factor, &current_symbol->address)) return error_code;
                    CreateDebris(script_tasks, subject_script, &debris_item);
                    return PushScriptStackData(subject_script, (byte*) &debris_item.id, TemporarySymbolType);
                } else {
                    error_code = PushScriptStackPointer(subject_script, current_symbol->address);
                }
            } else {
                return CouldNotFindTheSymbolError;
            }
            break;
        default:
            return InvalidOperationError;
    }
    return error_code;
}

int DereferenceData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol) {
    CalculationValues data_values = {0};
    StackData stack_symbol_0 = ExpandStackSymbol(script_tasks, subject_script, stack_symbol);
    uint64 address = 0; //pointer marker
    stack_symbol_0 = ExpandDebris(script_tasks, subject_script, stack_symbol_0);

    TemporarySymbol current_symbol = {0};
    current_symbol.address = *(uint64*) stack_symbol_0.data; //pointer marker
    uint64 *pc = &subject_script->values.program_counter;
    uint64 ending_position = *pc;

    switch(stack_symbol_0.type) {
        case CPointerType:
            switch(stack_symbol.type) {
                case SymbolType:
                    Symbol *this_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
                    if(!this_symbol) return CouldNotFindTheSymbolError;
                    if(this_symbol->dimension <= 0) return PointerDimensionZeroError;
                    current_symbol.dimension = this_symbol->dimension - 1;
                    current_symbol.array_dimension = this_symbol->array_dimension;
                    current_symbol.type = this_symbol->type;
                    current_symbol.sub_type = this_symbol->sub_type;
                    break;
                case TemporarySymbolType:
                    address = *(uint64*) stack_symbol.data; //pointer marker
                    int64 debris_index = FindDebris(subject_script, address);
                    if(debris_index == -1) return FailedToFindDebrisInDereferenceError;
                    TemporarySymbol *this_debris = subject_script->system.debris + debris_index;
                    if(this_debris->dimension <= 0) return PointerDimensionZeroError;
                    current_symbol.dimension = this_debris->dimension - 1;
                    current_symbol.array_dimension = this_debris->array_dimension;
                    current_symbol.type = this_debris->type;
                    current_symbol.sub_type = this_debris->sub_type;
                    current_symbol.address = *(uint64*) stack_symbol_0.data; //pointer marker
                    int error_code = DeleteDebris(script_tasks, subject_script, debris_index);
                    if(error_code) return error_code;
                    break;
                default:
                    return InvalidOperationError;
            }
            if(current_symbol.dimension <= 0) current_symbol.type = current_symbol.sub_type;
            current_symbol.size = GetDataTypeLength(current_symbol.type);
            current_symbol.settings |= IsCLike;
            break;
        default:
            StackData memory_data = GetStackDataFromMemory(script_tasks, subject_script, &ending_position);
            if(memory_data.error) return memory_data.error;
            current_symbol.type = memory_data.type;
            current_symbol.sub_type = memory_data.sub_type;
            current_symbol.size = memory_data.length;
    }
    CreateDebris(script_tasks, subject_script, &current_symbol); //maybe check if this is -1 in the future
    return PushScriptStackData(subject_script, (byte*) &current_symbol.id, TemporarySymbolType); //pointer marker
}

int EqualData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            if(stack_symbol_0.type == 0 || stack_symbol_0.type == NullType) data_values.val1_8_u = 1;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data == CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data == CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data == CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data == CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data == CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data == CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data == CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data == CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data == CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data == CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data == CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) == CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int NotEqualData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);

    switch(stack_symbol_1.type) {
        case 0:
        case NullType:
            data_values.val1_8_u = 1;
            if(stack_symbol_0.type == 0 || stack_symbol_0.type == NullType) data_values.val1_8_u = 0;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data != CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data != CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data != CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data != CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data != CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data != CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data != CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data != CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data != CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data != CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data != CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) != CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int NotData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0) {
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    
    switch(stack_symbol_0.type) {
        case 0:
        case NullType:
            data_values.val1_8_u = 1;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = !(*stack_symbol_0.data);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = !(*(uint16*) stack_symbol_0.data);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = !(*(uint32*) stack_symbol_0.data);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = !(*(uint64*) stack_symbol_0.data);
            break;
        case Int8Type:
            data_values.val1_8_u = !(*(char*) stack_symbol_0.data);
            break;
        case Int16Type:
            data_values.val1_8_u = !(*(int16*) stack_symbol_0.data);
            break;
        case Int32Type:
            data_values.val1_8_u = !(*(int32*) stack_symbol_0.data);
            break;
        case Int64Type:
            data_values.val1_8_u = !(*(int64*) stack_symbol_0.data);
            break;
        case FloatType:
            data_values.val1_8_u = !(*(float32*) stack_symbol_0.data);
            break;
        case DoubleType:
            data_values.val1_8_u = !(*(float64*) stack_symbol_0.data);
            break;
        case LongDoubleType:
            data_values.val1_8_u = !(*(float128*) stack_symbol_0.data);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = !GetPointerStackData(stack_symbol_0);
            break;
        default:
            return InvalidOperationError;
    }
    return PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
}

int GreaterThanData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data > CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data > CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data > CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data > CastToUnsignedInt64(stack_symbol_0);
            break;
        case CharType:
        case DataType:
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data > CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data > CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data > CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data > CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data > CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data > CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data > CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) > CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int LessThanData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
        case NullType:
            if(stack_symbol_0.type > 1) data_values.val1_8_u = 1;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data < CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data < CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data < CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data < CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data < CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data < CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data < CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data < CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data < CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data < CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data < CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) < CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int GreaterThanOrEqualToData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
        case NullType:
            if(stack_symbol_1.type <= 1) data_values.val1_8_u = 1;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data >= CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data >= CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data >= CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data >= CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data >= CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data >= CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data >= CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data >= CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data >= CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data >= CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data >= CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) >= CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int LessThanOrEqualToData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
            if(stack_symbol_0.type > 1) data_values.val1_8_u = 1;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data <= CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data <= CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data <= CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data <= CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data <= CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data <= CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data <= CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data <= CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data <= CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data <= CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data <= CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) <= CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int CastData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);

    switch(stack_symbol_1.type) {
        case DataType:
            switch(*stack_symbol_1.data) {
                case 0:
                    error_code = PushScriptStackData(subject_script, 0, 0);
                    break;
                case NullType:
                    error_code = PushScriptStackData(subject_script, 0, NullType);
                    break;
                case CharType:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, CharType);
                    break;
                case DataType:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, DataType);
                    break;
                case UnsignedInt8Type:
                    data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8_u, UnsignedInt8Type);
                    break;
                case UnsignedInt16Type:
                    data_values.val1_16_u = CastToUnsignedInt16(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16_u, UnsignedInt16Type);
                    break;
                case UnsignedInt32Type:
                    data_values.val1_32_u = CastToUnsignedInt32(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32_u, UnsignedInt32Type);
                    break;
                case UnsignedInt64Type:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64_u, UnsignedInt64Type);
                    break;
                case Int8Type:
                    data_values.val1_8 = CastToInt8(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_8, Int8Type);
                    break;
                case Int16Type:
                    data_values.val1_16 = CastToInt16(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_16, Int16Type);
                    break;
                case Int32Type:
                    data_values.val1_32 = CastToInt32(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_32, Int32Type);
                    break;
                case Int64Type:
                    data_values.val1_64 = CastToInt64(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.val1_64, Int64Type);
                    break;
                case FloatType:
                    data_values.f1_32 = CastToFloat32(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_32, FloatType);
                    break;
                case DoubleType:
                    data_values.f1_64 = CastToFloat64(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_64, DoubleType);
                    break;
                case LongDoubleType:
                    data_values.f1_128 = CastToFloat128(stack_symbol_0);
                    error_code = PushScriptStackData(subject_script, (byte*) &data_values.f1_128, LongDoubleType);
                    break;
                case PointerType:
                    data_values.val1_64_u = CastToUnsignedInt64(stack_symbol_0);
                    error_code = PushScriptStackPointer(subject_script, data_values.val1_64_u);
                    break;
                default:
                    return InvalidOperationError;
            }
            break;
        default:
            return CastedWithNonDataTypeError;
    }
    return error_code;
}

int ConnectData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol) {
    int error_code = 0;
    CalculationValues data_values = {0};
    int multiply_factor = bit_mode / 8;

    StackData stack_symbol_1 = ExpandStackDataNoDelete(script_tasks, subject_script, stack_symbol);

    switch(stack_symbol_0.type) {
        case ElementType:
            break;
        default:
            return InvalidElementTypeError;
    }

    uint64 elements_list;
    uint64 old_elements_list;
    TemporarySymbol temporary_symbol = {0};
    uint64 *pointers = 0;
    uint64 old_size = 0;
    uint64 list_pointer = 0;

    switch(stack_symbol.type) {
        case SymbolType:
            Symbol *temp_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
            if(temp_symbol) temporary_symbol.list_pointer = temp_symbol->list_pointer; else return SymbolNotFoundError;
            temporary_symbol.address = temp_symbol->address;
            temporary_symbol.list_pointer = temp_symbol->list_pointer;
            old_size = temp_symbol->size;
            temporary_symbol.size = temp_symbol->size;
            break;
        case TemporarySymbolType:
            TemporarySymbol *temp_debris = 0;
            int64 debris_index = FindDebris(subject_script, *(uint64*) stack_symbol.data); //pointer marker
            if(debris_index == -1) return DebrisNotFoundError;
            temp_debris = subject_script->system.debris + debris_index;
            temporary_symbol.list_pointer = temp_debris->list_pointer;
            temporary_symbol.address = temp_debris->address;
            temporary_symbol.settings = temp_debris->settings;
            old_size = temp_debris->size;
            DeleteDebris(script_tasks, subject_script, debris_index);
            break;
        case ObjectType:
            temporary_symbol.address = *(uint64*) stack_symbol.data; //pointer marker
            break;
        default:
            return NotASymbolOrDebrisError;
    }

    switch(stack_symbol_1.type) {
        case ObjectType:
            if(!temporary_symbol.address) return ObjectDoesNotExistError;
            Object *temp_object = (Object*) temporary_symbol.address;
            ObjectClassInformation *class_info = FindObjectClass(script_tasks, temp_object->class);
            temporary_symbol.settings |= HasGeniunePointer;
            temporary_symbol.settings |= IsCLike;
            if(!temp_object) return ObjectDoesNotExistError;
            uint64 element_string = *(uint64*) stack_symbol_0.data;
            byte *element_length = ReadScriptMemoryRange(script_tasks, subject_script, element_string, 1);
            if(!element_length) return NonExistentEnumMemberError;
            int64 method_index = WordIsObjectMethod(element_length + 1, *element_length);
            if(method_index != -1) {
                ObjectMethod object_method = root_object_methods[method_index];
                return object_method.callback(script_tasks, subject_script, temp_object);
            }
            Object *child_object = WordIsObjectChild(temp_object, element_length + 1, *element_length);
            if(child_object) return PushScriptStackData(subject_script, (byte*) &child_object, ObjectType);
            if(!class_info) return InvalidObjectClassError;
            int (*callback) (ScriptEnvironment*, Script*, Object*);
            byte *element_real_string = ReadScriptMemoryRange(script_tasks, subject_script, element_string + 1, (uint64) *element_length + 1);
            if(!element_real_string) return NonExistentEnumMemberError;
            callback = WordIsObjectCallback(script_tasks, class_info, element_real_string, *element_length);
            if(callback) {
                uint64 *callback_data = malloc(multiply_factor * 2); //pointer marker
                *callback_data = (uint64) temp_object;
                *(callback_data + 1) = (uint64) callback;
                return PushScriptStackData(subject_script, (byte*) callback_data, ObjectCallbackType);
            }
            temporary_symbol = class_info->content;
            if(temporary_symbol.type == CustomDataType) temporary_symbol.type = temporary_symbol.sub_type;
            stack_symbol_1.type = temporary_symbol.type;
            temporary_symbol.settings |= HasGeniunePointer;
            temporary_symbol.address = (uint64) temp_object->content;
    }

    switch(stack_symbol_1.type) {
        case StructType:
            pointers = (uint64*) ReadScriptMemoryRange(script_tasks, subject_script, temporary_symbol.list_pointer, multiply_factor * 4);
            if(!pointers) return StructListCouldNotBeFoundError; else {
                uint64 search_string_pointer = *(uint64*) stack_symbol_0.data;
                uint64 list_items = *pointers;
                uint64 list_elements = *(pointers + 1);
                uint64 list_data = *(pointers + 2);
                uint64 list_auxilary = *(pointers + 3);
                int64 list_index = -1;
                byte *search_length = ReadScriptMemoryRange(script_tasks, subject_script, search_string_pointer, 1);
                if(!search_length) return CouldNotReadMemoryError;
                byte *search_string = ReadScriptMemoryRange(script_tasks, subject_script, search_string_pointer + 1, (short) *search_length + 1);
                if(!search_string) return CouldNotReadMemoryError;
                uint64 byte_offset = 0;
                for(uint64 i = 0; i < list_items; i++) {
                    uint64 element_pointer = *(uint64*) ReadScriptMemoryRange(script_tasks, subject_script, list_elements + (multiply_factor * i), multiply_factor);
                    uint64 auxilary_pointer = *(uint64*) ReadScriptMemoryRange(script_tasks, subject_script, list_auxilary + (multiply_factor * i), multiply_factor);
                    byte *element_length = ReadScriptMemoryRange(script_tasks, subject_script, list_data + i, 1);
                    if(!element_length) return CouldNotReadMemoryError;

                    switch(*element_length) {
                        case StructType:
                            int64 get_size = GetRecursiveStructSize(script_tasks, subject_script, auxilary_pointer);
                            if(get_size == -1) return StructListCouldNotBeFoundError;
                            temporary_symbol.size = get_size;
                            temporary_symbol.type = StructType;
                            break;
                        case Bool8Type:
                        case Bool16Type:
                        case Bool32Type:
                        case Bool64Type:
                            temporary_symbol.type = *element_length;
                            temporary_symbol.size = GetDataTypeSize(temporary_symbol.type);
                            break;
                        default:
                            temporary_symbol.type = *element_length;
                            temporary_symbol.size = GetDataTypeLength(temporary_symbol.type); //be sure to add sub types and others with the auxilary list later
                    }
                    temporary_symbol.list_pointer = auxilary_pointer;
                    element_length = ReadScriptMemoryRange(script_tasks, subject_script, element_pointer, 1);
                    if(!element_length) return CouldNotReadMemoryError;
                    if(*element_length == *search_length) {
                        byte *element_string = ReadScriptMemoryRange(script_tasks, subject_script, element_pointer + 1, (short) *element_length + 1);
                        if(!element_string) return CouldNotReadMemoryError;
                        if(CompareSymbolStrings(search_string, element_string, (short) *element_length + 1)) {
                            list_index = i;
                            break;
                        }
                    }
                    byte_offset += temporary_symbol.size;
                }
                if(list_index != -1) {
                    temporary_symbol.settings |= IsCLike;
                    temporary_symbol.address += byte_offset;
                    CreateDebris(script_tasks, subject_script, &temporary_symbol);
                    return PushScriptStackData(subject_script, (byte*) &temporary_symbol.id, TemporarySymbolType);
                } else return StructElementNotFoundError;
            }
            break;
        case Bool8Type:
        case Bool16Type:
        case Bool32Type:
        case Bool64Type:
            elements_list = temporary_symbol.address;
            old_elements_list = elements_list;
            uint64 data_size = GetDataTypeSize(stack_symbol_1.type);
            byte stack_data_type = 0;
            uint64 bool_value = 0;
            byte *data_ptr = (byte*) elements_list;
            if(!(temporary_symbol.settings & HasGeniunePointer)) data_ptr = ReadScriptMemoryRange(script_tasks, subject_script, elements_list, data_size);
            if(!data_ptr) return CouldNotReadMemoryError;
            switch(data_size) {
                case 1:
                    stack_data_type = Bool8ContainerType;
                    bool_value = (uint64) *(byte*) data_ptr;
                    break;
                case 2:
                    stack_data_type = Bool16ContainerType;
                    bool_value = (uint64) *(uint16*) data_ptr;
                    break;
                case 4:
                    stack_data_type = Bool32ContainerType;
                    bool_value = (uint64) *(uint32*) data_ptr;
                    break;
                case 8:
                    stack_data_type = Bool64ContainerType;
                    bool_value = *(uint64*) data_ptr;
                    break;
                default:
                    return InvalidOperationError;
            }
            elements_list = temporary_symbol.list_pointer;
            for(uint64 i = 0; i < (data_size * 8); i++) {
                uint64 string_address = *(uint64*) ReadScriptMemoryRange(script_tasks, subject_script, elements_list + (i * multiply_factor), multiply_factor);
                if(string_address == 0) break;
                uint64 element_pointer = *(uint64*) stack_symbol_0.data;
                byte value_length = ReadScriptMemory(script_tasks, subject_script, string_address);
                byte element_length = ReadScriptMemory(script_tasks, subject_script, element_pointer);
                if(element_length == value_length) {
                    byte *value_string = ReadScriptMemoryRange(script_tasks, subject_script, element_pointer + 1, (int) value_length + 1);
                    byte *element_string = ReadScriptMemoryRange(script_tasks, subject_script, string_address + 1, (int) value_length + 1);
                    if(CompareSymbolStrings(value_string, element_string, (short) value_length + 1)) {
                        byte *data_buffer = malloc(GetDataTypeLength(stack_data_type));
                        *data_buffer = (byte) i;
                        if(temporary_symbol.settings & HasGeniunePointer) *(data_buffer + (bit_mode / 8) + 1) |= BoolHasGeniunePointer;
                        memcpy(data_buffer + 1, &old_elements_list, multiply_factor);
                        if(error_code = PushScriptStackData(subject_script, data_buffer, stack_data_type)) return error_code;
                        free(data_buffer);
                        return 0;
                    }
                }
            }
            return NonExistentEnumMemberError;
        case EnumType:
            elements_list = GetPointerStackData(stack_symbol_1);
            stack_symbol_1.data += multiply_factor;
            uint64 values_list = GetPointerStackData(stack_symbol_1);
            stack_symbol_1.data -= multiply_factor;
            byte *data_buffer = ReadScriptMemoryRange(script_tasks, subject_script, elements_list, multiply_factor);
            if(!data_buffer) return EnumListReadFailureError;
            uint64 length = *(uint64*) data_buffer;
            elements_list += multiply_factor;
            values_list += multiply_factor;
            for(uint64 i = 0; i < length; i++) {
                uint64 element_index = elements_list + (i * multiply_factor);
                uint64 element_pointer = GetPointerFromMemory(script_tasks, subject_script, &element_index);
                uint64 element_pointer_2 = *(uint64*) stack_symbol_0.data;
                byte value_length = ReadScriptMemory(script_tasks, subject_script, element_pointer);
                byte element_length = ReadScriptMemory(script_tasks, subject_script, element_pointer_2);
                if(element_length == value_length) {
                    byte *value_string = ReadScriptMemoryRange(script_tasks, subject_script, element_pointer + 1, (int) value_length + 1);
                    byte *element_string = ReadScriptMemoryRange(script_tasks, subject_script, element_pointer_2 + 1, (int) value_length + 1);
                    if(CompareSymbolStrings(value_string, element_string, (short) value_length + 1)) {
                        uint64 value_index = values_list + (i * multiply_factor);
                        uint64 value = GetPointerFromMemory(script_tasks, subject_script, &value_index);
                        return PushScriptStackData(subject_script, (byte*) &value, UnsignedInt64Type);
                    }
                }
            }
            return NonExistentEnumMemberError;
        case ObjectType:
            
            break;
        default:
            return InvalidOperationError;
    }

    return error_code;
}

int ArrayIndexData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol) {
    CalculationValues data_values = {0};
    StackData stack_symbol_1 = ExpandStackDataNoDelete(script_tasks, subject_script, stack_symbol);
    uint64 address = 0; //pointer marker
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);

    TemporarySymbol current_symbol = {0};
    current_symbol.address = *(uint64*) stack_symbol_1.data;
    uint64 *pc = &subject_script->values.program_counter;
    uint64 ending_position = *pc;

    switch(stack_symbol_1.type) {
        case CPointerType:
            switch(stack_symbol.type) {
                case SymbolType:
                    Symbol *this_symbol = GetSymbolFromStackData(subject_script, stack_symbol);
                    if(!(this_symbol->settings & IsArray)) return NotAnArrayError;
                    if(!this_symbol) return CouldNotFindTheSymbolError;
                    if(this_symbol->array_dimension <= 0) return PointerDimensionZeroError;
                    current_symbol.dimension = this_symbol->dimension;
                    current_symbol.array_dimension = this_symbol->array_dimension - 1;
                    current_symbol.type = this_symbol->type;
                    current_symbol.sub_type = this_symbol->sub_type;
                    break;
                case TemporarySymbolType:
                    address = *(uint64*) stack_symbol.data; //pointer marker
                    int64 debris_index = FindDebris(subject_script, address);
                    if(debris_index == -1) return FailedToFindDebrisInDereferenceError;
                    TemporarySymbol *this_debris = subject_script->system.debris + debris_index;
                    if(!(this_debris->settings & IsArray)) return NotAnArrayError;
                    if(this_debris->array_dimension <= 0) return PointerDimensionZeroError;
                    current_symbol.dimension = this_debris->dimension;
                    current_symbol.array_dimension = this_debris->array_dimension - 1;
                    current_symbol.type = this_debris->type;
                    current_symbol.sub_type = this_debris->sub_type;
                    int error_code = DeleteDebris(script_tasks, subject_script, debris_index);
                    if(error_code) return error_code;
                    break;
                default:
                    return InvalidOperationError;
            }
            current_symbol.settings |= IsCLike;
            current_symbol.settings |= IsArray;
            if(current_symbol.array_dimension <= 0) current_symbol.type = current_symbol.sub_type;
            current_symbol.size = GetDataTypeLength(current_symbol.type);
            current_symbol.address = *(uint64*) stack_symbol_1.data + (CastToUnsignedInt64(stack_symbol_0) * current_symbol.size); //pointer marker
            break;
        default:
            return NotACLikePointerError;
    }
    CreateDebris(script_tasks, subject_script, &current_symbol); //maybe check if this is -1 in the future
    return PushScriptStackData(subject_script, (byte*) &current_symbol.id, TemporarySymbolType); //pointer marker
}

int ElementCallbackData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol) {
    int error_code = 0;
    int multiply_factor = bit_mode / 8;

    StackData stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol);

    switch(stack_symbol_1.type) {
        case ObjectCallbackType:
            Object *object_address = (Object*) *(uint64*) stack_symbol_1.data;
            int (*callback) (ScriptEnvironment*, Script*, Object*);
            callback = (void*) (*((uint64*) stack_symbol_1.data + 1));
            if(object_address && callback) {
                PushScriptStackData(subject_script, stack_symbol_0.data, stack_symbol_0.type);
                return callback(script_tasks, subject_script, object_address);
            }
    }

    return PushScriptStackData(subject_script, 0, 0);
}

int LogicalAndData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};
    
    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
        case NullType:
            data_values.val1_8_u = 0;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data && CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data && CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data && CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data && CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data && CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data && CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data && CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data && CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data && CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data && CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data && CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) && CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int LogicalOrData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    CalculationValues data_values = {0};

    stack_symbol_0 = ExpandStackData(script_tasks, subject_script, stack_symbol_0);
    stack_symbol_1 = ExpandStackData(script_tasks, subject_script, stack_symbol_1);
    
    switch(stack_symbol_1.type) {
        case 0:
        case NullType:
            data_values.val1_8_u = CastToUnsignedInt8(stack_symbol_0) != 0;
            break;
        case CharType:
        case DataType:
        case UnsignedInt8Type:
            data_values.val1_8_u = *stack_symbol_1.data || CastToUnsignedInt8(stack_symbol_0);
            break;
        case UnsignedInt16Type:
            data_values.val1_8_u = *(uint16*) stack_symbol_1.data || CastToUnsignedInt16(stack_symbol_0);
            break;
        case UnsignedInt32Type:
            data_values.val1_8_u = *(uint32*) stack_symbol_1.data || CastToUnsignedInt32(stack_symbol_0);
            break;
        case UnsignedInt64Type:
            data_values.val1_8_u = *(uint64*) stack_symbol_1.data || CastToUnsignedInt64(stack_symbol_0);
            break;
        case Int8Type:
            data_values.val1_8_u = *(char*) stack_symbol_1.data || CastToInt8(stack_symbol_0);
            break;
        case Int16Type:
            data_values.val1_8_u = *(int16*) stack_symbol_1.data || CastToInt16(stack_symbol_0);
            break;
        case Int32Type:
            data_values.val1_8_u = *(int32*) stack_symbol_1.data || CastToInt32(stack_symbol_0);
            break;
        case Int64Type:
            data_values.val1_8_u = *(int64*) stack_symbol_1.data || CastToInt64(stack_symbol_0);
            break;
        case FloatType:
            data_values.val1_8_u = *(float32*) stack_symbol_1.data || CastToFloat32(stack_symbol_0);
            break;
        case DoubleType:
            data_values.val1_8_u = *(float64*) stack_symbol_1.data || CastToFloat64(stack_symbol_0);
            break;
        case LongDoubleType:
            data_values.val1_8_u = *(float128*) stack_symbol_1.data || CastToFloat128(stack_symbol_0);
            break;
        case PointerType:
        case CPointerType:
            //pointer marker
            data_values.val1_8_u = GetPointerStackData(stack_symbol_1) || CastToUnsignedInt64(stack_symbol_0);
            break;
    }
    return PushScriptStackData(subject_script, &data_values.val1_8_u, UnsignedInt8Type);
}

int HandleScriptOperation(ScriptEnvironment *script_tasks, Script *subject_script, byte bytecode, StackData stack_symbol_0, StackData stack_symbol_1) {
    int error_code = 0;
    switch(bytecode) {
        case AssignOperator:
        case AddAssignOperator:
        case SubtractAssignOperator:
        case MultiplyAssignOperator:
        case DivideAssignOperator:
        case RemainderAssignOperator:
        case ShiftLeftAssignOperator:
        case ShiftRightAssignOperator:
        case AndAssignOperator:
        case XorAssignOperator:
        case OrAssignOperator:
            if(stack_symbol_1.type == SymbolType) {
                Symbol *this_symbol = GetSymbolFromStackData(subject_script, stack_symbol_1);
                if(this_symbol) {
                    if(this_symbol->settings & IsConstant) return AttemptedModificationOfConstantError;
                }
            }
    }
    switch(bytecode) {
        case AddOperator:    
            return AddData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case SubtractOperator:
            return SubtractData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case MultiplyOperator:
            return MultiplyData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case DivideOperator:
            return DivideData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case RemainderOperator:
            return RemainderOfData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case BitwiseShiftLeftOperator:
            return ShiftDataLeft(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case BitwiseShiftRightOperator:
            return ShiftDataRight(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case BitwiseAndOperator:
            return BitwiseAndData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case BitwiseOrOperator:
            return BitwiseOrData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case BitwiseXorOperator:
            return BitwiseXorData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case AssignOperator:
            return AssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case AddAssignOperator:
            return AddAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case SubtractAssignOperator:
            return SubtractAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case MultiplyAssignOperator:
            return MultiplyAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case DivideAssignOperator:
            return DivideAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case RemainderAssignOperator:
            return RemainderAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case ShiftLeftAssignOperator:
            return ShiftLeftAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case ShiftRightAssignOperator:
            return ShiftRightAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case AndAssignOperator:
            return AndAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case XorAssignOperator:
            return XorAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case OrAssignOperator:
            return OrAssignData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case EqualOperator:
            return EqualData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case NotEqualOperator:
            return NotEqualData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case GreaterThanOperator:
            return GreaterThanData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case LessThanOperator:
            return LessThanData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case GreaterThanOrEqualToOperator:
            return GreaterThanOrEqualToData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case LessThanOrEqualToOperator:
            return LessThanOrEqualToData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case LogicalAndOperator:
            return LogicalAndData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case LogicalOrOperator:
            return LogicalOrData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case IntelligentAndOperator:
            return LogicalAndData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case CastOperator:
            return CastData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case ConnectorOperator:
            return ConnectData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case ArrayIndexOperator:
            return ArrayIndexData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case ElementCallbackOperator:
            return ElementCallbackData(script_tasks, subject_script, stack_symbol_0, stack_symbol_1);
        case CommaOperator:
            if(error_code = PushScriptStackData(subject_script, stack_symbol_1.data, stack_symbol_1.type)) return error_code;
            error_code = PushScriptStackData(subject_script, stack_symbol_0.data, stack_symbol_0.type);
    }
    return error_code;
}

int CalculateScriptOperation(ScriptEnvironment *script_tasks, Script *subject_script, byte bytecode) {
    StackData stack_symbol_0 = {0};
    StackData stack_symbol_1 = {0};

    if(bytecode != 255 && bytecode != CommaOperator) {
        stack_symbol_0 = PopScriptStackData(subject_script);
        StackData *stack_data_array = 0;
        stack_symbol_1 = PopScriptStackData(subject_script);
        if(!stack_symbol_0.data || !stack_symbol_1.data) return StackEmptyError;
        return HandleScriptOperation(script_tasks, subject_script, bytecode, stack_symbol_0, stack_symbol_1);
    }

    return 0;
}

int ProcessScriptOperationPrefix(ScriptEnvironment *script_tasks, Script *subject_script, byte bytecode) {
    StackData stack_symbol_0 = {0};

    if(bytecode != 255) {
        stack_symbol_0 = PopScriptStackData(subject_script);
        if(!stack_symbol_0.data) return StackEmptyError;
    }

    switch(bytecode) {
        case UnaryMinusOperator:    
            return NegateData(script_tasks, subject_script, stack_symbol_0);
        case UnaryPlusOperator:
            return PositateData(script_tasks, subject_script, stack_symbol_0);
        case BitwiseNotOperator:
            return BitwiseNotData(script_tasks, subject_script, stack_symbol_0);
        case LogicalNotOperator:
            return NotData(script_tasks, subject_script, stack_symbol_0);
        case DereferenceOperator:
            return DereferenceData(script_tasks, subject_script, stack_symbol_0);
        case AddressOfOperator:
            return AddressOfData(script_tasks, subject_script, stack_symbol_0);
        case PrefixIncrementOperator:
            return PrefixIncrementData(script_tasks, subject_script, stack_symbol_0);
        case PrefixDecrementOperator:
            return PrefixDecrementData(script_tasks, subject_script, stack_symbol_0);
    }
    return 0;
}

int ProcessScriptOperationSuffix(ScriptEnvironment *script_tasks, Script *subject_script, byte bytecode) {
    StackData stack_symbol_0 = {0};

    if(bytecode != 255) {
        stack_symbol_0 = PopScriptStackData(subject_script);
        if(!stack_symbol_0.data) return StackEmptyError;
    }

    switch(bytecode) {
        case IncrementOperator:
            return IncrementData(script_tasks, subject_script, stack_symbol_0);
        case DecrementOperator:
            return DecrementData(script_tasks, subject_script, stack_symbol_0);
    }
    return 0;
}

