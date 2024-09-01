StackData TokenIsDefinition(Script *subject_script, byte *token) {
    StackData ret_val = {0};
    ret_val.error = 1;
    for(int i = 0; i < subject_script->values.definition_count; i++) {
        Keyword *current_kw = (subject_script->values.definitions + i);
        if(strcmp(token, current_kw->name) == 0) {
            ret_val.type = current_kw->type;
            ret_val.length = current_kw->size;
            ret_val.data = current_kw->value;
            ret_val.error = 0;
        }
    }
    return ret_val;
}

StackData TokenIsKeyword(ScriptEnvironment *script_tasks, byte *token) {
    StackData ret_val = {0};
    ret_val.error = 1;
    for(int i = 0; i < script_tasks->keyword_count; i++) {
        Keyword *current_kw = (script_tasks->keywords + i);
        if(strcmp(token, current_kw->name) == 0) {
            ret_val.type = current_kw->type;
            ret_val.length = current_kw->size;
            ret_val.data = current_kw->value;
            ret_val.error = 0;
        }
    }
    return ret_val;
}

byte TokenIsOpcode(byte *token) {
    for(int i = 0; i < megascript_standard_call_count; i++) if(strcmp(token, megascript_default_standard_calls[i].name) == 0) return megascript_default_standard_calls[i].value;
    return 0;
}

byte TokenIsDataType(byte *token) {
    for(int i = 0; i < megascript_data_type_count; i++) if(strcmp(token, megascript_default_data_types[i].name) == 0) return megascript_default_data_types[i].value;
    return 0;
}

byte TokenIsPrecursor(byte *token) {
    for(int i = 0; i < megascript_precursors_count; i++) if(strcmp(token, megascript_default_precursors[i].name) == 0) return megascript_default_precursors[i].value;
    return 0;
}

uint16 *TokenIsStandardCall(ScriptEnvironment *script_tasks, byte *token) {
    uint16 *ret_val = 0;
    //for(int i = 0; i < script_tasks->standard_calls_count; i++) if(strcmp(token, (((StandardCall*) script_tasks->standard_calls + i))->name) == 0) {
    for(int i = 0; i < standard_call_count; i++) if(strcmp(token, default_standard_calls[i].name) == 0) {
        ret_val = malloc(2);
        *ret_val = (uint16) i;
    }
    return ret_val;
}

//byte CheckForPrefixOperator(byte *copied_string, long long unsigned int *string_position, long long unsigned int source_length)

int ClearWhiteSpace(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int ret_val = 0;
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(current_char) {
            case ';':
            case ' ':
            case '\n':
            case '\t':
                ret_val = 1;
                ending_point++;
                break;
            default:
                *string_position = ending_point;
                token_unchanged = 0;
        }
    }
    return ret_val;
}

byte CheckForWordPrefix(byte *copied_string, uint64 *string_position, uint64 source_length) {
    byte ret_val = 0;
    int token_unchanged = 1;
    byte current_char = *(copied_string + *string_position);
    switch(current_char) {
        case '*':
            (*string_position)++;
            return DereferenceOperator;
        case '$':
            (*string_position)++;
            return DereferenceOperator;
        case '@':
            (*string_position)++;
            return AddressOfOperator;
        case '!':
            (*string_position)++;
            return LogicalNotOperator;
        case '+':
            (*string_position)++;
            return UnaryPlusOperator;
        case '-':
            (*string_position)++;
            return UnaryMinusOperator; //oops almost missed that lol
        case '~':
            (*string_position)++;
            return BitwiseNotOperator;
        default:
            return 0;
    }
}

byte CheckForOpeningBracket(byte *copied_string, uint64 *string_position, uint64 source_length) {
    byte ret_val = 0;
    int token_unchanged = 1;
    byte current_char = *(copied_string + *string_position);
    switch(current_char) {
        case '(':
            (*string_position)++;
            return CallOperator;
        case '[':
            (*string_position)++;
            return ArrayOperator;
        case '{':
            (*string_position)++;
            return DynamicOperator;
        default:
            return 0;
    }
}

byte CheckForClosingBracket(byte *copied_string, uint64 *string_position, uint64 source_length) {
    byte ret_val = 0;
    int token_unchanged = 1;
    byte current_char = *(copied_string + *string_position);
    switch(current_char) {
        case ')':
            (*string_position)++;
            return CallEndOperator;
        case ']':
            (*string_position)++;
            return ArrayEndOperator;
        case '}':
            (*string_position)++;
            return DynamicEndOperator;
        default:
            return 0;
    }
}

byte *CheckForWord(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    byte *ret_val = malloc(1);
    *ret_val = '\0';
    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(current_char) {
            case ')':
            case ']':
            case '}':
            case '{':
            case ';':
            case '~':
            case '>':
            case '<':
            case '$':
            case '@':
            case '"':
            case '(':
            case '[':
            case '=':
            case '+':
            case '-':
            case '*':
            case '/':
            case '|':
            case '^':
            case '&':
            case '%':
            case ',':
            case '.':
            case '`':
            case ':':
            case '\n':
            case '\'':
            case '\\':
            case ' ':
            case 0:
                token_unchanged = 0;
                *string_position = ending_point;
                return ret_val;
            default:
                ending_point++;
                ret_val = realloc(ret_val, (ending_point - (*string_position)) + 1);
                *(ret_val + ((ending_point - (*string_position)) - 1)) = current_char;
                *(ret_val + (ending_point - (*string_position))) = '\0';
                //*(ret_val + (ending_point - (*string_position)) + 1) = '\0';
        }
    }
    return ret_val;
}

byte GetEscapedCharacter(byte escape_label) {
    byte ret_val = escape_label;

    switch(escape_label) {
        case 'n':
            ret_val = '\n';
            break;
        case '\\':
            ret_val = '\\';
            break;
        case '0':
            ret_val = '\0';
            break;
        case 't':
            ret_val = '\t';
            break;
        case '\"':
            ret_val = '\"';
            break;
        case '\'':
            ret_val = '\'';
            break;
        case '?':
            ret_val = '\?';
            break;
        case 'a':
            ret_val = '\a';
            break;
        case 'b':
            ret_val = '\b';
            break;
        case 'f':
            ret_val = '\f';
            break;
        case 'r':
            ret_val = '\r';
            break;
        case 'v':
            ret_val = '\v';
            break;
    }

    return ret_val;
}

byte *DecodeString(byte *copied_string, uint64 *string_position, uint64 source_length, uint64 *return_length) {
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    uint64 string_length = 0;
    byte *ret_val = malloc(1);
    *ret_val = '\0';
    byte escape_next_character = 0;
    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(current_char) {
            case '\"':
                if(*string_position == ending_point || !escape_next_character) {
                    token_unchanged = 0;
                    if(current_char == '\"') ending_point++;
                    *string_position = ending_point;
                    *return_length = string_length;
                    break;
                }
            case '\\':
                if(!escape_next_character) {
                    escape_next_character = 1;
                    ending_point++;
                    break;
                }
            default:
                ending_point++;
                string_length++;
                ret_val = realloc(ret_val, string_length + 1);
                switch(escape_next_character) {
                    case 0:
                        *(ret_val + (string_length - 1)) = current_char;
                        break;
                    default:
                        *(ret_val + (string_length - 1)) = GetEscapedCharacter(current_char);
                        escape_next_character = 0;
                }
                *(ret_val + (string_length)) = '\0';
        }
    }
    return ret_val;
}

byte DecodeCharacter(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    byte ret_val = '\0';
    byte escape_next_character = 0;
    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(current_char) {
            case '\'':
                if(*string_position == ending_point || !escape_next_character) {
                    token_unchanged = 0;
                    ending_point++;
                    *string_position = ending_point;
                    break;
                }
            case '\\':
                if(!escape_next_character) {
                    escape_next_character = 1;
                    ending_point++;
                    break;
                }
            default:
                ending_point++;
                switch(escape_next_character) {
                    case 0:
                        ret_val = current_char;
                        break;
                    default:
                        ret_val = GetEscapedCharacter(current_char);
                        escape_next_character = 0;
                }
        }
    }
    return ret_val;
}

//useful function I found on stackoverflow
long long unsigned int ipow(long long unsigned int base, long long unsigned int exp) {
    long long unsigned int result = 1;
    for (;;) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }
    return result;
}

int ProgressDigitByBase(NumberContainer *ret_val, byte *compare_lower, uint64 current_length, byte current_char, byte base) {
    if(current_char >= base) {
        ret_val->error = InvalidDigitError;
        return 0;
    }

    uint64 numerical_spot = current_length - 1;
    uint64 current_digit = ipow(base, numerical_spot);

    if(!(*compare_lower)) {
        byte current_64 = (MegascriptMax64Bit / current_digit) % base;
        if(current_64 < current_char) {
            ret_val->error = TemporaryNumberSizeError;
            return 0;
            //ret_val->type = PseudoNumberContainerType;
        } else if(current_64 > current_char) {
            *compare_lower = 1;
        }
    }

    if(ret_val->is_signed) {
        ret_val->whole_number = (int64) ret_val->whole_number + ((int64) current_char * (int64) current_digit);
    } else {
        ret_val->whole_number += (current_char * current_digit);
    }

    return 0;
}

int ProgressDecimalByBase(NumberContainer *ret_val, byte *compare_lower, uint64 current_length, uint64 decimal_length, byte current_char, byte base) {
    uint64 numerical_spot = 0;
    uint64 current_digit = 0;
    switch(decimal_length) {
        case 0:
            if(current_char >= base) {
                ret_val->error = InvalidDigitError;
                return 0;
            }

            numerical_spot = current_length - 1;
            current_digit = ipow(base, numerical_spot);

            if(!(*compare_lower)) {
                byte current_64 = (MegascriptMax64Bit / current_digit) % base;
                if(current_64 < current_char) {
                    ret_val->error = TemporaryNumberSizeError;
                    return 0;
                } else if(current_64 > current_char) {
                    *compare_lower = 1;
                }
            }

            if(ret_val->is_signed) {
                ret_val->whole_number = (int64) ret_val->whole_number + ((int64) current_char * (int64) current_digit);
            } else {
                ret_val->whole_number += (current_char * current_digit);
            }
            break;
        default:
            numerical_spot = decimal_length;
            current_digit = ipow(base, numerical_spot);

            ret_val->floating_point += ((long double) current_char / (long double) current_digit);
    }

    return 0;
}

NumberContainer DecodeNumber(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    NumberContainer ret_val = {0};
    ret_val.type = IntNumberContainerType;
    byte *string_buffer = malloc(1);
    *string_buffer = '\0';
    int64 decimal_position = -1;
    //NumberContainer *ret_val = malloc(sizeof(NumberContainer)); //this will be for if a non dynamic variable fails

    switch(*(copied_string + ending_point)) {
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        default:
            ret_val.error = NotANumberError;
            free(string_buffer);
            return ret_val;
    }

    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(*(copied_string + ending_point)) {
            case '.':
                decimal_position = ending_point - (*string_position);
                ret_val.type = FloatNumberContainerType;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'a':
            case 'A':
            case 'b':
            case 'B':
            case 'c':
            case 'C':
            case 'd':
            case 'D':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            case 'h':
            case 'H':
            case 'i':
            case 'I':
            case 'j':
            case 'J':
            case 'k':
            case 'K':
            case 'l':
            case 'L':
            case 'm':
            case 'M':
            case 'n':
            case 'N':
            case 'o':
            case 'O':
            case 'p':
            case 'P':
            case 'q':
            case 'Q':
            case 'r':
            case 'R':
            case 's':
            case 'S':
            case 't':
            case 'T':
            case 'u':
            case 'U':
            case 'v':
            case 'V':
            case 'w':
            case 'W':
            case 'x':
            case 'X':
            case 'y':
            case 'Y':
            case 'z':
            case 'Z':
                ending_point++;
                string_buffer = realloc(string_buffer, (ending_point - (*string_position)) + 1);
                *(string_buffer + ((ending_point - (*string_position)) - 1)) = current_char;
                *(string_buffer + (ending_point - (*string_position))) = '\0';
                break;
            default:
                token_unchanged = 0;
        }
    }

    uint64 buffer_length = (ending_point - (*string_position));
    uint64 buffer_position = 0;
    byte number_type = Base10NumberStringType;
    byte base = 10;
    ret_val.is_signed = 1;
    ret_val.bit_length = bit_mode;

    if(decimal_position > -1) {
        ret_val.type = megascript_default_float_type;
        
        if(CheckSetting(DynamicFloatResizing)) switch(buffer_length - 1 > 6 /*__FLT_DIG__*/) {
            case 0:
                ret_val.type = FloatNumberContainerType;
                break;
            default:
                switch(buffer_length - 1 > 15 /*__DBL_DIG__*/) {
                    case 0:
                        ret_val.type = DoubleNumberContainerType;
                        break;
                    default:
                        switch(buffer_length - 1 > 18 /*__LDBL_DIG__*/) {
                            case 0:
                                ret_val.type = LongDoubleNumberContainerType;
                                break;
                            default:
                                ret_val.type = LongDoubleNumberContainerType;
                        }
                }
        }
    }

    if(buffer_length > 2 && *string_buffer == '0' && *(string_buffer + 1) == 'x') {
        buffer_position = 2;
        number_type = Base16NumberStringType;
        base = 16;
    }else if(buffer_length > 3 && *string_buffer == '3' && *(string_buffer + 1) == '2' && *(string_buffer + 2) == 'x') {
        buffer_position = 3;
        number_type = Base32NumberStringType;
        base = 32;
    } else if((buffer_length > 2 && *(string_buffer + 1) == 'c') || (buffer_length > 3 && *(string_buffer + 2) == 'c')) {
        byte chr1 = *string_buffer;
        byte chr2 = 0;
        if(*(string_buffer + 1) > 47 && *(string_buffer + 1) < 58) {
            chr2 = *(string_buffer + 1);
        }
        buffer_position = 2;
        switch(chr2) {
            case 0:
                base = chr1 - 48;
                break;
            default:
                base = ((chr1 - 48) * 10) + (chr2 - 48);
                buffer_position++;
        }
        number_type = CustomNumberStringType;
    } else if(decimal_position < 0) {
        char looping = 1;
        while(buffer_length > 1 && looping) {
            switch(*(string_buffer + (buffer_length - 1))) {
                case 'u':
                case 'U':
                    ret_val.is_signed = 0;
                    buffer_length--;
                    break;
                case 'b':
                case 'B':
                    ret_val.bit_length = 8;
                    ret_val.is_static = 1;
                    buffer_length--;
                    break;
                case 'w':
                case 'W':
                    ret_val.bit_length = 16;
                    ret_val.is_static = 1;
                    buffer_length--;
                    break;
                case 'd':
                case 'D':
                    ret_val.bit_length = 32;
                    ret_val.is_static = 1;
                    buffer_length--;
                    break;
                case 'q':
                case 'Q':
                    ret_val.bit_length = 64;
                    ret_val.is_static = 1;
                    buffer_length--;
                    break;
                case 'p':
                case 'P':
                    ret_val.type = PseudoNumberContainerType;
                    buffer_length--;
                    break;
                default:
                    looping = 0;
            }
        }
    } else {
        char looping = 1;
        while(buffer_length > 1 && looping) {
            switch(*(string_buffer + (buffer_length - 1))) {
                case 'f':
                case 'F':
                    ret_val.type = FloatNumberContainerType;
                    ret_val.bit_length = 32;
                    buffer_length--;
                    break;
                case 'd':
                case 'D':
                    ret_val.type = DoubleNumberContainerType;
                    ret_val.bit_length = 64;
                    buffer_length--;
                    break;
                case 'l':
                case 'L':
                    ret_val.type = LongDoubleNumberContainerType;
                    ret_val.bit_length = 128;
                    buffer_length--;
                    break;
                case 'p':
                case 'P':
                    ret_val.type = PseudoNumberContainerType;
                    buffer_length--;
                    break;
                default:
                    looping = 0;
            }
        }
    }

    int64 corrected_decimal_position = decimal_position - buffer_position;
    byte compare_lower = 0;
    int max_digits = 0;

    if(base) {
        long long unsigned int current_cut = MegascriptMax64Bit;
        while(current_cut = current_cut / base) {
            max_digits++;
        }

        if(max_digits > buffer_length - buffer_position) compare_lower = 1;
    }

    while(buffer_position < buffer_length) {
        uint64 current_length = buffer_length - buffer_position;
        uint64 decimal_length = 0;
        if(decimal_position > -1) {
            current_length -= current_length - corrected_decimal_position;
            current_length -= buffer_position;
            if(buffer_position > decimal_position) {
                decimal_length = buffer_position - decimal_position;
            }
        }

        byte current_char = *(string_buffer + buffer_position);
        switch(current_char) {
            case '.':
                buffer_position++;
                continue;
        }

        if(current_char < 91 && current_char > 64) current_char = current_char + 32; //transform the ascii characters into numbers
        if(current_char > 57 && current_char < 123) current_char -= 39;
        current_char -= 48;

        switch(decimal_length <= 0) {
            case 0:
                ProgressDecimalByBase(&ret_val, &compare_lower, current_length, decimal_length, current_char, base);
                break;
            default:
                ProgressDigitByBase(&ret_val, &compare_lower, current_length, current_char, base);
        }

        buffer_position++;
    }

    ret_val.floating_point += ret_val.whole_number;

    *string_position = ending_point;
    free(string_buffer);
    
    return ret_val;
}

int ContinueToNextLine(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int token_unchanged = 1;
    uint64 ending_point = *string_position;
    while(token_unchanged && ending_point < source_length) {
        byte current_char = *(copied_string + ending_point);
        switch(current_char) {
            case '\n':
                ending_point++;
            case '\0':
                *string_position = ending_point;
                return 0;
            default:
                ending_point++;
        }
    }
    if(ending_point >= source_length) *string_position = ending_point;
    return UnexpectedEndOfScriptError;
}

byte CheckForAssignment(byte *copied_string, uint64 *string_position, uint64 source_length) {
    int token_unchanged = 1;
    int character_found = 0;
    byte ret_val = 0;
    byte current_char = *(copied_string + *string_position);
    byte temp_char;
    switch(current_char) {
        case '+':
            if(*string_position + 1 < source_length) {
                temp_char = *(copied_string + (*string_position + 1));
                switch(temp_char) {
                    case '=':
                        *string_position += 2;
                        return AddAssignOperator;
                    case '+':
                        *string_position += 2;
                        return IncrementOperator;
                    default:
                        return 0;
                }
            }
            break;
        case '-':
            if(*string_position + 1 < source_length) {
                temp_char = *(copied_string + (*string_position + 1));
                switch(temp_char) {
                    case '=':
                        *string_position += 2;
                        return SubtractAssignOperator;
                    case '-':
                        *string_position += 2;
                        return DecrementOperator;
                }
            }
            break;
        case '*':
        case '/':
        case '|':
        case '^':
        case '&':
        case '%':
            if(*string_position + 1 < source_length) if((*(copied_string + *string_position + 1)) == '=') {
                *string_position += 2;
                switch(current_char) {
                    case '*':
                        return MultiplyAssignOperator;
                    case '/':
                        return DivideAssignOperator;
                    case '|':
                        return OrAssignOperator;
                    case '^':
                        return XorAssignOperator;
                    case '&':
                        return AndAssignOperator;
                    case '%':
                        return RemainderAssignOperator;
                }
            }
            break;
        case '<':
        case '>':
            if(*string_position + 2 < source_length) if((*(copied_string + *string_position + 1)) == current_char && (*(copied_string + *string_position + 2)) == '=') {
                *string_position += 3;
                switch(current_char) {
                    case '>':
                        return ShiftRightAssignOperator;
                    default:
                        return ShiftLeftAssignOperator;
                }
            }
    }
    return 0;
}

byte CheckForOperatorSign(byte *copied_string, uint64 *string_position, uint64 source_length) {
    uint64 ending_point = *string_position;
    byte current_char = *(copied_string + ending_point);
    byte assign_operator = CheckForAssignment(copied_string, string_position, source_length);
    if(assign_operator && assign_operator != EqualOperator) return assign_operator;
    int operation_length = 1;
    switch(current_char) {
        case '+':
            *string_position = *string_position + 1;
            return AddOperator;
        case '-':
            if((*(copied_string + (ending_point + 1)) == '>')) {
                *string_position = *string_position + 2;
                return PointerConnectorOperator;
            }
            *string_position = *string_position + 1;
            return SubtractOperator;
        case '.':
            *string_position = *string_position + 1;
            return ConnectorOperator;
        case '*':
            *string_position = *string_position + 1;
            return MultiplyOperator;
        case '/':
            *string_position = *string_position + 1;
            return DivideOperator;
        case '^':
            *string_position = *string_position + 1;
            return BitwiseXorOperator;
        case '%':
            *string_position = *string_position + 1;
            return RemainderOperator;
        case '~':
            *string_position = *string_position + 1;
            return BitwiseNotOperator;
        case ',':
            *string_position = *string_position + 1;
            return CommaOperator;
        case '?':
            *string_position = *string_position + 1;
            return TernaryOneOperator;
        case '&':
        case '|':
            ending_point++;
            if(ending_point + 1 < source_length) if((*(copied_string + ending_point) == current_char)) {
                ending_point++;
                operation_length = 2;
            }
            *string_position = ending_point;
            switch(operation_length) {
                case 2:
                    switch(current_char) {
                        case '&':
                            if(ending_point + 2 < source_length) {
                                if(*(copied_string + ending_point) == current_char) {
                                    (*string_position)++;
                                    return IntelligentAndOperator;
                                } else return LogicalAndOperator;
                            } else {
                                return LogicalAndOperator;
                            }
                        case '|':
                            return LogicalOrOperator;
                        default:
                            return 0;
                    }
                    break;
                default:
                    switch(current_char) {
                        case '&':
                            return BitwiseAndOperator;
                        case '|':
                            return BitwiseOrOperator;
                        default:
                            return 0; //just realized that zero should be returned if there's an error so it doesn't think it got an operator //UnknownOperatorError;
                    }
            }
        case '=':
        case '!':
            if(ending_point + 1 < source_length && (*(copied_string + (ending_point + 1)) == '=')) {
                *string_position = *string_position + 2;
                switch(current_char) {
                    case '!':
                        return NotEqualOperator;
                    default:
                        return EqualOperator;
                }
            } else if(current_char == '=') {
                (*string_position)++;
                return AssignOperator;
            } else {
                return 0;
            }
        case '<':
        case '>':
            if(ending_point + 1 < source_length && ((*(copied_string + (ending_point + 1)) == '=') || (*(copied_string + (ending_point + 1)) == current_char))) {
                byte is_current_char = (*(copied_string + (ending_point + 1)) == current_char);
                *string_position += 2;
                switch(is_current_char) {
                    case 0:
                        switch(current_char) {
                            case '<':
                                return LessThanOrEqualToOperator;
                            case '>':
                                return GreaterThanOrEqualToOperator;
                            default:
                                return 0;
                        }
                    default:
                        switch(current_char) {
                            case '<':
                                return BitwiseShiftLeftOperator;
                            case '>':
                                return BitwiseShiftRightOperator;
                            default:
                                return 0;
                        }
                }
            } else {
                (*string_position)++;
                switch(current_char) {
                    case '<':
                        return LessThanOperator;
                    case '>':
                        return GreaterThanOperator;
                    default:
                        return 0;
                }
            }
        case '\0':
        default:
            return 0;
    }
    return 0;
}

byte CheckForAssignmentPrefix(byte *copied_string, uint64 *string_position, uint64 source_length) {
    byte token = CheckForAssignment(copied_string, string_position, source_length);
    switch(token) {
        case IncrementOperator:
            return PrefixIncrementOperator;
        case DecrementOperator:
            return PrefixDecrementOperator;
    }
    return 0;
};

int AddMegaScriptBytes(byte *token, uint64 *byte_count, int bytes_to_add, uint64 *chunk_count, byte **script_code) {
    int starting_position = *byte_count;
    *byte_count += bytes_to_add;
    if(*chunk_count < *byte_count) {
        *chunk_count += allocation_chunk_size;
        *script_code = realloc(*script_code, *chunk_count);
        if(!script_code) return 0;
    }
    memcpy((*script_code) + starting_position, token, bytes_to_add);
    return 1;
}

uint64 AddVariableToScope(ScopeManager *this_scope, byte *variable_string, int length, int type) {
    uint64 current_id = this_scope->id;
    this_scope->id = this_scope->id + 1;
    this_scope->variables = realloc(this_scope->variables, this_scope->id * sizeof(UnparsedVariable));
    UnparsedVariable *current_variable = (this_scope->variables + current_id);
    current_variable->length = length;
    current_variable->name = malloc(length + 1);
    current_variable->type = type;
    *(current_variable->name + length) = '\0';
    memcpy(current_variable->name, variable_string, length);
    return current_id;
}

uint64 AddLabelToScope(ScopeManager *this_scope, byte *variable_string, int length, uint64 address) {
    uint64 variable_id = AddVariableToScope(this_scope, variable_string, length, LabelUnparsedVariableType);
    UnparsedVariable *current_variable = (this_scope->variables + variable_id);
    current_variable->address = address;
    return variable_id;
}

int64 GetVariableId(ScopeManager *this_scope, byte *variable_string, int length) {
    uint64 last_match = -1;
    for(int i = 0; i < this_scope->id; i++) {
        UnparsedVariable this_variable = *(this_scope->variables + i);
        if(this_variable.length == length) {
            int str_cmp = 0;
            for(int j = 0; j < length; j++) {
                if(*(this_variable.name + j) != *(variable_string + j)) str_cmp = 1;
            }
            if(str_cmp == 0) {
                last_match = i;
            }
        }
    }
    return last_match;
}

int TokenIsTypedef(ScopeManager *this_scope, byte *name, uint64 length) {
    int64 var_id = GetVariableId(this_scope, name, length);
    if(var_id != -1) {
        if((this_scope->variables + var_id)->type == TypedefUnparsedVariableType) return var_id;
    }
    return -1;
}

int ParseMegaScriptStatement(uint64, uint64*, uint64*, uint64*, byte**, byte*, ScopeManager*, ScopeManager**, uint64*);
int ParseMegaScriptOperation(ScriptEnvironment*, Script*, uint64, uint64*, uint64*, uint64*, byte**, byte*, ScopeManager*, ScopeManager**, uint64*, ScopeManager**, uint32);
int ParseMegaScriptFunctionCall(uint64, uint64*, uint64*, uint64*, byte**, byte*, ScopeManager*, ScopeManager**, uint64*);
void *ReadScriptMemoryRange(ScriptEnvironment*, Script*, uint64, uint64);
byte ReadScriptMemory(ScriptEnvironment*, Script*, uint64);
int WriteScriptMemory(ScriptEnvironment*, Script*, uint64, byte);

int AddPrefixToCode(byte prefix, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
    return 1;
}

int AddPointerToCode(uint64 variable_id, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    byte temp_bytes[8];
    switch(bit_mode) {
        case 32:
            unsigned int *temp_bytes_32 = (unsigned int*) temp_bytes;
            *temp_bytes_32 = (unsigned int) variable_id;
            AddMegaScriptBytes(temp_bytes, byte_count, 4, chunk_count, script_code);
            break;
        case 64:
            uint64 *temp_bytes_64 = (uint64*) temp_bytes;
            *temp_bytes_64 = variable_id;
            AddMegaScriptBytes(temp_bytes, byte_count, 8, chunk_count, script_code);
            break;
        default:
            return BitSizeError;
    }
    return 0;
}

int AddSymbolToCode(byte *token, uint64 current_length, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    byte current_length_8 = (byte) (current_length - 1);
    AddMegaScriptBytes(&current_length_8, byte_count, 1, chunk_count, script_code);
    AddMegaScriptBytes(token, byte_count, current_length, chunk_count, script_code);
    return 0;
}

int ParseMegaScriptNumber(NumberContainer returned_number, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope) {
    int error_code = 0;

    byte byte_size = 0;
    byte number_opcode = 0;
    byte number_buffer[16];

    switch(returned_number.type) {
        case IntNumberContainerType:
            byte bit8 = 255U;
            uint16 bit16 = 65535U;
            unsigned int bit32 = 4294967295U;

            if(returned_number.is_signed) {
                number_opcode = SignedIntegerOpcode;
                bit8 = 127U;
                bit16 = 32767U;
                bit32 = 2147483647U;
            } else {
                number_opcode = IntegerOpcode;
            }

            if(CheckSetting(DynamicIntegerResizing) && !returned_number.is_static) {
                switch(returned_number.whole_number > bit8) {
                    case 0:
                        byte_size = 1;
                        //number_buffer = malloc(1);
                        *number_buffer = (byte) returned_number.whole_number;
                        break;
                    default:
                        switch(returned_number.whole_number > bit16) {
                            case 0:
                                byte_size = 2;
                                //number_buffer = malloc(2);
                                memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                                break;
                            default:
                                switch(returned_number.whole_number > bit32) {
                                    case 0:
                                        byte_size = 4;
                                        //number_buffer = malloc(4);
                                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                                        break;
                                    default:
                                        byte_size = 8;
                                        //number_buffer = malloc(8);
                                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);

                                }
                        }
                }
            } else {
                switch(returned_number.bit_length) {
                    case 8:
                        byte_size = 1;
                        *number_buffer = (byte) returned_number.whole_number;
                        break;
                    case 16:
                        byte_size = 2;
                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                        break;
                    case 32:
                        byte_size = 4;
                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                        break;
                    case 64:
                        byte_size = 8;
                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                        break;
                    default:
                        byte_size = returned_number.bit_length / 8;
                        memcpy(number_buffer, (byte*) (&returned_number.whole_number), byte_size);
                }
            }
            break;
        case FloatNumberContainerType:
            byte_size = 4;
            number_opcode = FloatOpcode;
            //number_buffer = malloc(4);
            float temp_number2 = (float) returned_number.floating_point;
            memcpy(number_buffer, (byte*) (&temp_number2), byte_size);
            break;
        case DoubleNumberContainerType:
            byte_size = 8;
            number_opcode = FloatOpcode;
            //number_buffer = malloc(8);
            double temp_number3 = (double) returned_number.floating_point;
            memcpy(number_buffer, (byte*) (&temp_number3), byte_size);
            break;
        case LongDoubleNumberContainerType:
            byte_size = 16;
            number_opcode = FloatOpcode;
            //number_buffer = malloc(16);
            memcpy(number_buffer, (byte*) (&returned_number.floating_point), byte_size);
            break;
    }

    AddMegaScriptBytes(&number_opcode, byte_count, 1, chunk_count, script_code);
    AddMegaScriptBytes(&byte_size, byte_count, 1, chunk_count, script_code);
    AddMegaScriptBytes(number_buffer, byte_count, byte_size, chunk_count, script_code);
                        
    return error_code;
};

int ParseMegaScriptCreateVariable(ScriptEnvironment*, Script*, byte*, uint64, byte, byte, uint64, uint64*, uint64*, uint64*, byte**, byte*, ScopeManager*, ScopeManager**, uint64*, ScopeManager**);
int EnterMegaScriptScope(ScopeManager**, ScopeManager**, uint64*);
int ExitMegaScriptScope(ScopeManager**, ScopeManager**, uint64*);

int CheckForCastNotation(ScriptEnvironment *script_tasks, Script *subject_script, byte *script_source, uint64 *byte_count, uint64 *chunk_count, byte **script_code, uint64 *string_position, uint64 source_length) {
    int ret_val = 0;
    uint64 original_position = *string_position;
    (*string_position)++;
    byte *token = CheckForWord(script_source, string_position, source_length);
    if(token && *token != '\0') {
        ClearWhiteSpace(script_source, string_position, source_length);
        byte data_type = TokenIsDataType(token);
        if(data_type && *(script_source + *string_position) == ')') {
            (*string_position)++;
            ClearWhiteSpace(script_source, string_position, source_length);
            byte temp_byte = DataTypeOpcode;
            AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
            AddMegaScriptBytes(&data_type, byte_count, 1, chunk_count, script_code);
            temp_byte = CastOperator;
            AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
        } else {
            *string_position = original_position;
            ret_val = 1;
        }
    } else {
        *string_position = original_position;
        ret_val = 1;
    }
    free(token);
    return ret_val;
}

int CheckForEmptyCall(ScriptEnvironment *script_tasks, Script *subject_script, byte *script_source, uint64 *byte_count, uint64 *chunk_count, byte **script_code, uint64 *string_position, uint64 source_length) {
    if(*(script_source + (*string_position)) == '(') {
        uint64 old_position = *string_position;
        (*string_position)++;
        ClearWhiteSpace(script_source, string_position, source_length);
        if(*(script_source + (*string_position)) == ')') {
            (*string_position)++;
            return 1;
        } else {
            *string_position = old_position;
        }
    }
    return 0;
}

int ParseStandardCallParameters(ScriptEnvironment *script_tasks, Script *subject_script, byte call_arguments, byte standard_call_parse_settings, byte *script_source, uint64 *byte_count, uint64 *chunk_count, uint64 *current_scope, ScopeManager **scopes, byte **script_code, uint64 *string_position, uint64 source_length, ScopeManager *this_scope, ScopeManager **labels) {
    int error_code = 0;
    byte remove_ending_parenthesis = 0;
    if(standard_call_parse_settings & SnipsParenthesis) {
        if(*(script_source + *string_position) == '(') {
            remove_ending_parenthesis = 1;
            (*string_position)++;
            if(CheckForCastNotation(script_tasks, subject_script, script_source, byte_count, chunk_count, script_code, string_position, source_length)) remove_ending_parenthesis = 0;
        }
    }
    if(standard_call_parse_settings & CallEntersScope) if(error_code = EnterMegaScriptScope(&this_scope, scopes, current_scope)) return error_code;
    if(!CheckForEmptyCall(script_tasks, subject_script, script_source, byte_count, chunk_count, script_code, string_position, source_length) && call_arguments) {
        error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0);
    } else if(call_arguments) return TooFewStandardCallArgumentsError;
    if(!(standard_call_parse_settings & SkipStopStandardCallBytecode)) {
        byte opcode = StopStandardCallOpcode;
        AddMegaScriptBytes(&opcode, byte_count, 1, chunk_count, script_code);
    }
    if(remove_ending_parenthesis) if(*(script_source + *string_position) == ')') (*string_position)++;
    return error_code;
}

int ParseMegaScriptStandardCall(ScriptEnvironment *script_tasks, Script *subject_script, byte opcode, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels) {
    uint64 enum_offset = *byte_count;
    uint64 enum_chunk = *chunk_count;
    AddMegaScriptBytes(&opcode, byte_count, 1, chunk_count, script_code);
    ClearWhiteSpace(script_source, string_position, source_length);
    int error_code = 0;
    byte *token = 0;
    byte standard_call_parse_settings = 0;
    uint64 old_position = 0;
    switch(opcode) {
        case FunctionOpcode:
            old_position = *string_position;
            token = CheckForWord(script_source, string_position, source_length);
            if(token) {
                AddSymbolToCode(token, (*string_position) - (old_position), byte_count, chunk_count, script_code);
                free(token);
            } else {
                return MissingVariableNameError;
            }

            error_code = EnterMegaScriptScope(&this_scope, scopes, current_scope);
            if(error_code) return error_code;
            ClearWhiteSpace(script_source, string_position, source_length);

            if(*(script_source + (*string_position)) == '(') {
                error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0);
            } else {
                return MissingFunctionDefineBracketError; //make it so you can do it without brackets for no arguments maybe
            }
            break;
        case LambdaFunctionOpcode:
            old_position = *string_position;
            error_code = EnterMegaScriptScope(&this_scope, scopes, current_scope);
            if(error_code) return error_code;
            ClearWhiteSpace(script_source, string_position, source_length);

            if(*(script_source + (*string_position)) == '(') {
                error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0);
            } else {
                return MissingFunctionDefineBracketError; //make it so you can do it without brackets for no arguments maybe
            }
            break;
        case NewOpcode:
            token = CheckForWord(script_source, string_position, source_length);
            if(token) {
                byte data_type = TokenIsDataType(token);
                byte old_data_type = data_type;
                free(token);
                ClearWhiteSpace(script_source, string_position, source_length);
                old_position = *string_position;
                switch(data_type) {
                    case EnumType:
                        *byte_count = enum_offset;
                        *chunk_count = enum_chunk;
                        data_type = EnumOpcode;
                    case StructType:
                    case Bool8Type:
                    case Bool16Type:
                    case Bool32Type:
                    case Bool64Type:
                        token = CheckForWord(script_source, string_position, source_length);
                        if(token) {
                            AddMegaScriptBytes(&data_type, byte_count, 1, chunk_count, script_code);
                            switch(data_type) {
                                case Bool8Type:
                                case Bool16Type:
                                case Bool32Type:
                                case Bool64Type:
                                case StructType:
                                    AddVariableToScope(this_scope, token, *string_position - old_position, TypedefUnparsedVariableType);
                            }
                            AddSymbolToCode(token, (*string_position) - (old_position), byte_count, chunk_count, script_code);
                            free(token);

                            ClearWhiteSpace(script_source, string_position, source_length);

                            error_code = EnterMegaScriptScope(&this_scope, scopes, current_scope);
                            if(error_code) return error_code;
                            ClearWhiteSpace(script_source, string_position, source_length);

                            switch(old_data_type) {
                                case EnumType:
                                case Bool8Type:
                                case Bool16Type:
                                case Bool32Type:
                                case Bool64Type:
                                    error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, IsInsideEnum);
                                    break;
                                case StructType:
                                    error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, IsInsideStruct);
                            }
                        } else return MissingVariableNameError;
                        break;
                    default:
                        return NewMissingDataTypeError;
                }
            } else return NewMissingDataTypeError;
            break;
        case StopOpcode:
            error_code = ExitMegaScriptScope(&this_scope, scopes, current_scope);
            break;
        case ElseIfOpcode:
        case ElseOpcode:
            error_code = ExitMegaScriptScope(&this_scope, scopes, current_scope);
        case WhileOpcode:
        case IfOpcode:
        case ForOpcode:
            standard_call_parse_settings |= CallEntersScope;
            standard_call_parse_settings |= SkipStopStandardCallBytecode;
            standard_call_parse_settings |= SnipsParenthesis;
        case SizeofOpcode:
        case WaitOpcode:
        case PrintOpcode:
        case ReturnOpcode:
        case JumpOpcode:
        case SpaceofOpcode:
        case DeleteOpcode:
        case LoadOpcode:
            byte parameter_count = 0;
            for(int i = 0; i < bytecode_information_count; i++) {
                if(bytecode_information[i].code == opcode) parameter_count = bytecode_information[i].arguments;
            }
            error_code = ParseStandardCallParameters(script_tasks, subject_script, parameter_count, standard_call_parse_settings, script_source, byte_count, chunk_count, current_scope, scopes, script_code, string_position, source_length, this_scope, labels);
    }
    return error_code;
}

int AddBracketToCode(byte prefix, int *call_brackets, int *square_brackets, int *curly_brackets, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    switch(prefix) {
        case CallOperator:
            (*call_brackets)++;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
        case ArrayOperator:
            (*square_brackets)++;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
        case DynamicOperator:
            (*curly_brackets)++;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
    }
    return 1;
}

int AddClosingBracketToCode(byte prefix, int *call_brackets, int *square_brackets, int *curly_brackets, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    switch(prefix) {
        case CallEndOperator:
            if(*call_brackets <= 0) return 0;
            (*call_brackets)--;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
        case ArrayEndOperator:
            if(*square_brackets <= 0) return 0;
            (*square_brackets)--;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
        case DynamicEndOperator:
            if(*curly_brackets <= 0) return 0;
            (*curly_brackets)--;
            AddMegaScriptBytes(&prefix, byte_count, 1, chunk_count, script_code);
            break;
    }
    return 1;
}

int ConvertStackDataToCode(StackData, uint64*, uint64*, byte**);

int HandleParserDefinition(StackData keyword_data, uint64 *byte_count, uint64 *chunk_count, byte **script_code, uint32 operation_settings) {
    if(operation_settings & IsArrayInitializer) {
        uint64 initializer_value = CastToUnsignedInt64(keyword_data); //pointer marker
        byte initializer_byte = ArrayInitializerSymbolConstructorCode;
        AddMegaScriptBytes(&initializer_byte, byte_count, 1, chunk_count, script_code);
        AddMegaScriptBytes((byte*) &initializer_value, byte_count, 8, chunk_count, script_code); //pointer marker
    } else {
        ConvertStackDataToCode(keyword_data, byte_count, chunk_count, script_code);
    }
    return 0;
}

int ParseMegaScriptOperation(ScriptEnvironment *script_tasks, Script *subject_script, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels, uint32 operation_settings) {
    
    //this function will be used for all operations performed that will then be assigned to a variable, or passed as a function argument
    //example: the "1 + 2" in a = 1 + 2
    int error_code = 0;
    int call_brackets = 0;
    int square_brackets = 0;
    int curly_brackets = 0;
    byte first_word = 1;
    byte element_callback_byte = 0;

    while(!error_code) {
        ClearWhiteSpace(script_source, string_position, source_length);
        byte existing_prefix = 1;
        while(!(operation_settings & IsArrayInitializer) && existing_prefix != 0 && (*string_position) < source_length) {
            byte prefix;
            existing_prefix = 0;
            prefix = CheckForAssignmentPrefix(script_source, string_position, source_length);
            if(prefix) existing_prefix = AddPrefixToCode(prefix, byte_count, chunk_count, script_code);
            prefix = CheckForWordPrefix(script_source, string_position, source_length);
            if(prefix) existing_prefix = AddPrefixToCode(prefix, byte_count, chunk_count, script_code);
            prefix = CheckForOpeningBracket(script_source, string_position, source_length);
            if(prefix) {
                uint64 original_position = *string_position;
                byte *token = CheckForWord(script_source, string_position, source_length);
                if(prefix == CallOperator && token && *token != '\0') {
                    ClearWhiteSpace(script_source, string_position, source_length);
                    byte data_type = TokenIsDataType(token);
                    if(data_type && *(script_source + *string_position) == ')') {
                        (*string_position)++;
                        ClearWhiteSpace(script_source, string_position, source_length);
                        byte temp_byte = DataTypeOpcode;
                        AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                        AddMegaScriptBytes(&data_type, byte_count, 1, chunk_count, script_code);
                        temp_byte = CastOperator;
                        AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                    } else {
                        *string_position = original_position;
                        existing_prefix = AddBracketToCode(prefix, &call_brackets, &square_brackets, &curly_brackets, byte_count, chunk_count, script_code);
                    }
                } else {
                    *string_position = original_position;
                    existing_prefix = AddBracketToCode(prefix, &call_brackets, &square_brackets, &curly_brackets, byte_count, chunk_count, script_code);
                }
                free(token);
            }
            prefix = CheckForClosingBracket(script_source, string_position, source_length);
            if(prefix) {
                existing_prefix = AddClosingBracketToCode(prefix, &call_brackets, &square_brackets, &curly_brackets, byte_count, chunk_count, script_code);
                if(existing_prefix) {
                    (*string_position)--;
                    return 0;
                }
            }
            ClearWhiteSpace(script_source, string_position, source_length);
        } //function aaaa(int b) return b stop print(aaaa(2))

        byte word_found = 0;
        uint64 original_position = *string_position;
        byte first_character = *(script_source + (*string_position));
        byte *token = CheckForWord(script_source, string_position, source_length); //we'll want to check if the word is a number of some sort first

        if(token && *token != '\0') {
            word_found = 1;
            byte opcode = 0;
            StackData keyword_data = {0};
            uint16 *standard_call_id = 0; //make it skip these checkers if it's a union element
            
            if(!(operation_settings & LastWasUnion) && (opcode = TokenIsOpcode(token))) {
                if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                switch(opcode) {
                    case NewAliasOpcode:
                        *string_position = original_position;
                        opcode = NewOpcode;
                        break;
                    default:
                        ClearWhiteSpace(script_source, string_position, source_length);
                }
                if(*(script_source + *string_position) == '(' && opcode == FunctionOpcode) opcode = LambdaFunctionOpcode;
                error_code = ParseMegaScriptStandardCall(script_tasks, subject_script, opcode, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels);
            } else if(!(operation_settings & LastWasUnion) && (standard_call_id = TokenIsStandardCall(script_tasks, token))) {
                if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                opcode = StandardCallOpcode;
                AddMegaScriptBytes(&opcode, byte_count, 1, chunk_count, script_code);
                AddMegaScriptBytes((byte*) standard_call_id, byte_count, 2, chunk_count, script_code);
                free(token);
                error_code = ParseStandardCallParameters(script_tasks, subject_script, default_standard_calls[*standard_call_id].parameters, 0, script_source, byte_count, chunk_count, current_scope, scopes, script_code, string_position, source_length, this_scope, labels);
                free(standard_call_id);
            } else if(!(operation_settings & LastWasUnion) && (opcode = TokenIsDataType(token))) {
                if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                free(token);
                error_code = ParseMegaScriptCreateVariable(script_tasks, subject_script, 0, 0, operation_settings, opcode, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels);
            } else if(!(operation_settings & LastWasUnion) && !(keyword_data = TokenIsKeyword(script_tasks, token)).error) {
                //make sure to configure keywords and definitions to convert properly to 
                free(token);
                HandleParserDefinition(keyword_data, byte_count, chunk_count, script_code, operation_settings);
            } else if(!(operation_settings & LastWasUnion) && !(keyword_data = TokenIsDefinition(subject_script, token)).error) {
                free(token);
                HandleParserDefinition(keyword_data, byte_count, chunk_count, script_code, operation_settings);
            } else if(!(operation_settings & LastWasUnion) && TokenIsTypedef(this_scope, token, *string_position - original_position) != -1) {
                error_code = ParseMegaScriptCreateVariable(script_tasks, subject_script, token, *string_position - original_position, operation_settings, CustomDataType, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels);
            } else {
                uint64 str_length = *string_position - original_position;
                uint64 original_original = *string_position;
                *string_position = original_position;
                NumberContainer returned_number = DecodeNumber(script_source, string_position, source_length);
                if(operation_settings & IsArrayInitializer) {
                    free(token);
                    if(returned_number.error) return ComplexOperationPerformedInArrayInitializerError; //pointer marker
                    byte initializer_byte = ArrayInitializerSymbolConstructorCode;
                    AddMegaScriptBytes(&initializer_byte, byte_count, 1, chunk_count, script_code);
                    AddMegaScriptBytes((byte*) &returned_number.whole_number, byte_count, 8, chunk_count, script_code);
                } else if(returned_number.error) {
                    if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                    *string_position = original_original;
                    byte variable_opcode = VariableOpcode;
                    if(operation_settings & LastWasUnion) {
                        variable_opcode = ElementOpcode;
                        element_callback_byte = 1;
                        operation_settings ^= LastWasUnion;
                    } else {
                        element_callback_byte = 0;
                    }
                    if(operation_settings & IsInsideEnum && first_word) {
                        variable_opcode = EnumElementOpcode;
                        first_word = 0;
                    }
                    AddMegaScriptBytes(&variable_opcode, byte_count, 1, chunk_count, script_code);
                    error_code = AddSymbolToCode(token, str_length, byte_count, chunk_count, script_code);
                    free(token);
                    token = 0;
                    if(error_code) return error_code;
                    ClearWhiteSpace(script_source, string_position, source_length);
                    if(*(script_source + (*string_position)) == '(') {
                        if(element_callback_byte) {
                            byte temp_byte = ElementCallbackOperator;
                            AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                        }
                        if(operation_settings & IsInsideEnum) return FunctionCallInsideEnumError;
                        original_position = (*string_position)++;
                        ClearWhiteSpace(script_source, string_position, source_length);
                        if(*(script_source + (*string_position)) == ')') {
                            (*string_position)++;
                            byte call_byte = CallOperator;
                            if(operation_settings & LastWasUnion) {
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                                call_byte = IntegerOpcode;
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                                call_byte = 1;
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                                call_byte = 0;
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                                call_byte = CallEndOperator;
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                            } else {
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                                call_byte = CallEndOperator;
                                AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
                            }
                        } else {
                            *string_position = original_position;
                            error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0);
                            if(error_code) return error_code;
                        }
                    }
                } else {
                    error_code = ParseMegaScriptNumber(returned_number, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope);
                }
            }
        } else if(first_character == ':' && (operation_settings & StopAtTernary)) {
            (*string_position)++;
            byte call_byte = TernaryTwoOperator;
            AddMegaScriptBytes(&call_byte, byte_count, 1, chunk_count, script_code);
            return 0;
        } else if(first_character == '\\' && *(script_source + (*string_position) + 1) == '\\') {
            *string_position += 2;
            error_code = ContinueToNextLine(script_source, string_position, source_length);
        } else if(first_character == '\\' && *(script_source + (*string_position) + 1) == '*') {
            *string_position += 2;
            while(1) {
                (*string_position)++;
                byte current_char = *(script_source + (*string_position));
                if(current_char == '*') {
                    byte second_char = *(script_source + (*string_position) + 1);
                    if(second_char == '\\') {
                        *string_position += 2;
                        break;
                    } else if(second_char == '\0') {
                        (*string_position)++;
                        break;
                    }
                } else if(current_char == '\0') {
                    break;
                }
            }
        } else if(first_character == '\\') {
            return InvalidSyntaxError;
        } else if(first_character == '\"') {
            word_found = 1;
            *string_position = (*string_position) + 1;
            original_position = *string_position;
            uint64 str_length = 0;
            byte *string_buffer = DecodeString(script_source, string_position, source_length, &str_length);
            if(string_buffer) {
                byte opcode = StringOpcode;
                AddMegaScriptBytes(&opcode, byte_count, 1, chunk_count, script_code);
                error_code = AddPointerToCode(str_length, byte_count, chunk_count, script_code);
                AddMegaScriptBytes(string_buffer, byte_count, str_length, chunk_count, script_code);
                free(string_buffer);
            } else {
                string_buffer = 0;
                return UnfinishedStringError;
            }
        } else if(first_character == '\'') {
            *string_position = (*string_position) + 1;
            word_found = 1;
            byte new_character = DecodeCharacter(script_source, string_position, source_length);
            byte opcode = CharacterOpcode;
            AddMegaScriptBytes(&opcode, byte_count, 1, chunk_count, script_code);
            AddMegaScriptBytes(&new_character, byte_count, 1, chunk_count, script_code);
        }

        existing_prefix = 1;
        ClearWhiteSpace(script_source, string_position, source_length);

        while(existing_prefix && (*string_position) < source_length) {
            byte prefix = *(script_source + *string_position);
            byte temp_byte;
            existing_prefix = 0;
            switch(prefix) {
                case '[':
                    temp_byte = ArrayIndexOperator;
                    AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                    temp_byte = CallOperator;
                    AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                    existing_prefix = 1;
                    (*string_position)++;
                    ClearWhiteSpace(script_source, string_position, source_length);
                    if(error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0)) return error_code;
                    if(*(script_source + *string_position) != ']') return MissingArrayIndexClosingBracketError;
                    temp_byte = CallEndOperator;
                    AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                    (*string_position)++;
                    break;
            }
        }
        
        if(word_found && !error_code) {
            existing_prefix = 1;

            while(existing_prefix != 0 && (*string_position) < source_length) {
                byte prefix;
                existing_prefix = 0;
                prefix = CheckForClosingBracket(script_source, string_position, source_length);
                if(prefix) {
                    existing_prefix = AddClosingBracketToCode(prefix, &call_brackets, &square_brackets, &curly_brackets, byte_count, chunk_count, script_code);
                    if(!existing_prefix) {
                        (*string_position)--;
                        return 0;
                    }
                }
                ClearWhiteSpace(script_source, string_position, source_length);
            }

            ClearWhiteSpace(script_source, string_position, source_length);
            byte existing_prefix = 1;
            while(existing_prefix != 0 && (*string_position) < source_length) {
                byte prefix;
                existing_prefix = 0;
                uint64 prev_pos = *string_position;
                prefix = CheckForAssignment(script_source, string_position, source_length);
                switch(prefix) {
                    case IncrementOperator:
                    case DecrementOperator:
                        if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                        existing_prefix = AddPrefixToCode(prefix, byte_count, chunk_count, script_code);
                        ClearWhiteSpace(script_source, string_position, source_length);
                        break;
                    default:
                        *string_position = prev_pos;
                }
            }
            
            ClearWhiteSpace(script_source, string_position, source_length);
            byte operator_code = CheckForOperatorSign(script_source, string_position, source_length);

            if(operator_code) {
                if(operation_settings & IsArrayInitializer) return ComplexOperationPerformedInArrayInitializerError;
                AddMegaScriptBytes(&operator_code, byte_count, 1, chunk_count, script_code);
                ClearWhiteSpace(script_source, string_position, source_length);
                switch(operator_code) {
                    case TernaryOneOperator:
                        if(error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, StopAtTernary)) return error_code;
                        if(error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, StopAtTernary)) return error_code;
                        break;
                    case CommaOperator:
                        first_word = 1;
                        break;
                    case ConnectorOperator:
                    case PointerConnectorOperator:
                        operation_settings |= LastWasUnion;
                }
            } else {
                return 0;
            }
        } else {
            break;
        }
    }

    return error_code;
}

int ParseMegaScriptCreateVariable(ScriptEnvironment *script_tasks, Script *subject_script, byte *custom_data_string, uint64 custom_data_string_length, byte variable_settings, byte datatype, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels) {
    int error_code = 0;
    byte looping = 0;
    byte temp_byte = ConstructSymbolOpcode;
    AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);

    looping = 1;
    temp_byte = PointerMarkerSymbolConstructorCode;
    while(looping) {
        ClearWhiteSpace(script_source, string_position, source_length);
        byte first_character = *(script_source + (*string_position));

        switch(first_character) {
            case '$':
                AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);
                (*string_position)++;
                break;
            default:
                looping = 0;
        }
    }

    byte *token_data = malloc(2);
    if(variable_settings & IsInsideStruct) *token_data = (byte) CreateStructElementSymbolConstructorCode; else *token_data = (byte) CreateVariableOpcode;
    *(token_data + 1) = datatype;
    AddMegaScriptBytes(token_data, byte_count, 2, chunk_count, script_code);
    free(token_data);

    if(custom_data_string) AddSymbolToCode(custom_data_string, custom_data_string_length, byte_count, chunk_count, script_code);
    
    ClearWhiteSpace(script_source, string_position, source_length);
    uint64 old_position = *string_position;
    token_data = CheckForWord(script_source, string_position, source_length);

    if(token_data) {
        if(TokenIsOpcode(token_data)) error_code = StandardCallRedefinitionError;
        else if(TokenIsDataType(token_data)) error_code = StandardDataTypeRedefinitionError;
        
        if(!error_code) {
            AddSymbolToCode(token_data, (*string_position) - (old_position), byte_count, chunk_count, script_code);

            ClearWhiteSpace(script_source, string_position, source_length);
        }

        free(token_data);
    } else {
        error_code = MissingVariableNameError;
    }

    looping = 1;
    while(looping) {
        ClearWhiteSpace(script_source, string_position, source_length);
        byte first_character = *(script_source + (*string_position));

        if(first_character != '[') looping = 0; else {
            (*string_position)++;
            error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, IsArrayInitializer);
            first_character = *(script_source + (*string_position));
            if(first_character != ']') return MissingEndingArrayInitializerBracketError;
            (*string_position)++;
        }
    }

    temp_byte = EndSymbolConstructorCode;
    AddMegaScriptBytes(&temp_byte, byte_count, 1, chunk_count, script_code);

    return error_code;
}

int EnterMegaScriptScope(ScopeManager **main_scope, ScopeManager **scopes, uint64 *current_scope) {
    int error_code = 0;

    ScopeManager *last_scope = 0;
    ScopeManager *this_scope = 0;
    UnparsedVariable *last_variable = 0;
    UnparsedVariable *this_variable = 0;

    *current_scope = *current_scope + 1;
    *scopes = realloc(*scopes, (*current_scope + 1) * sizeof(ScopeManager));
    last_scope = (*scopes + (*current_scope - 1));
    this_scope = (*scopes + *current_scope);
    *main_scope = this_scope;
    *this_scope = (ScopeManager) {0};
    this_scope->id = last_scope->id;
    this_scope->variables = malloc((this_scope->id) * sizeof(UnparsedVariable));
    for(int i = 0; i < this_scope->id; i++) {
        this_variable = (this_scope->variables + i);
        last_variable = (last_scope->variables + i);
        this_variable->length = last_variable->length;
        this_variable->name = malloc(last_variable->length + 1);
        this_variable->type = last_variable->type;
        *(this_variable->name + this_variable->length) = '\0';
        memcpy(this_variable->name, last_variable->name, last_variable->length);
    }

    return error_code;
}

int ExitMegaScriptScope(ScopeManager **main_scope, ScopeManager **scopes, uint64 *current_scope) {
    int error_code = 0;

    ScopeManager *last_scope = 0;
    ScopeManager *this_scope = 0;
    UnparsedVariable *last_variable = 0;
    UnparsedVariable *this_variable = 0;

    if(*current_scope <= 0) return InvalidScopeError;
    this_scope = (*scopes + *current_scope);
    for(int i = 0; i < this_scope->id; i++) {
        this_variable = (this_scope->variables + i);
        free(this_variable->name);
        this_variable->name = 0;
    }
    free(this_scope->variables);
    this_scope->variables = 0;
    *current_scope = *current_scope - 1;
    *scopes = realloc(*scopes, (*current_scope + 1) * sizeof(ScopeManager));
    *main_scope = (*scopes + *current_scope);

    return error_code;
}

StackData PreExecuteOperationCode(ScriptEnvironment *script_tasks, Script *subject_script, uint64 *old_position, uint64 token_length, byte **token, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels) {
    ScriptEnvironment temporary_environment;
    temporary_environment.memory = *script_code + *byte_count;
    *old_position = *byte_count;
    StackData generated_data = {0};
    if(generated_data.error = ParseMegaScriptOperation(script_tasks, subject_script, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels, 0)) return generated_data;
    uint64 offset = 0;
    temporary_environment.memory_size = (*byte_count - *old_position) + 1;
    byte prev = subject_script->rules.settings;
    subject_script->rules.settings |= CanIgnoreMemoryProtection;
    generated_data = GetStackDataFromMemory(&temporary_environment, subject_script, &offset);
    subject_script->rules.settings = prev;
    *token = realloc(*token, token_length + 1);
    *(*token + token_length) = '\0';
    return generated_data;
}

int CreateSourceDefinition(ScriptEnvironment *script_tasks, Script *subject_script, Keyword **keyword_array, uint64 *keyword_count, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels) {
    int error_code = 0;
    ClearWhiteSpace(script_source, string_position, source_length);
    uint64 old_position = *string_position;
    byte *token = CheckForWord(script_source, string_position, source_length);
    uint64 token_length = *string_position - old_position;

    StackData keyword_token = TokenIsKeyword(script_tasks, token); //problem
    if(keyword_token.error) {
        *keyword_array = realloc(*keyword_array, (*keyword_count + 1) * sizeof(Keyword));
        StackData generated_data = PreExecuteOperationCode(script_tasks, subject_script, &old_position, token_length, &token, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels);
        if(generated_data.error) return generated_data.error;
        uint64 data_size = GetDataTypeLength(generated_data.type);
        byte *new_buffer = malloc(data_size);
        memcpy(new_buffer, generated_data.data, data_size);
        *(*keyword_array + *keyword_count) = (Keyword) {generated_data.type, data_size, token, new_buffer};
        (*keyword_count)++;
        *byte_count = old_position;
        error_code = 0;
    } else {
        int64 keyword_index = -1;
        for(uint64 i = 0; i < *keyword_count; i++) {
            token = realloc(token, token_length + 1);
            *(token + token_length) = '\0';
            token_length++;
            Keyword *current_keyword = *keyword_array + i;
            byte matches = 1;
            for(uint64 j = 0; j < token_length; j++) {
                if(*(token + j) != *(current_keyword->name + j)) {
                    matches = 0;
                    break;
                }
            }
            if(matches) {
                keyword_index = i;
                break;
            }
        }
        if(keyword_index != -1) {
            Keyword *top_keyword = *keyword_array + keyword_index;
            StackData generated_data = PreExecuteOperationCode(script_tasks, subject_script, &old_position, token_length, &token, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels);
            uint64 data_size = GetDataTypeLength(generated_data.type);
            byte *new_buffer = realloc(top_keyword->value, data_size);
            memcpy(new_buffer, generated_data.data, data_size);
            *(*keyword_array + keyword_index) = (Keyword) {generated_data.type, data_size, top_keyword->name, new_buffer};
            *byte_count = old_position;
        }
        free(token);
        error_code = 0;
    }
    return 0;
}

int ParsePreprocessorCommand(ScriptEnvironment *script_tasks, Script *subject_script, uint64 source_length, uint64 *string_position, uint64 *byte_count, uint64 *chunk_count, byte **script_code, byte *script_source, ScopeManager *this_scope, ScopeManager **scopes, uint64 *current_scope, ScopeManager **labels) {
    int error_code = LabelParsingError;

    ClearWhiteSpace(script_source, string_position, source_length);
    byte *ptoken = CheckForWord(script_source, string_position, source_length);
    byte opcode = 0;
    uint64 sub_string_position = 0;
    if(ptoken) {
        uint64 old_position = 0;
        byte *token = 0;
        switch(TokenIsPrecursor(ptoken)) {
            case LabelPrecursor:
                ClearWhiteSpace(script_source, string_position, source_length);
                old_position = *string_position;
                token = CheckForWord(script_source, string_position, source_length);
                if(token) {
                    int64 variable_id = GetVariableId(*labels, token, *string_position - old_position);
                    if(variable_id >= 0) {
                        error_code = TakenLabelError;
                    } else {
                        variable_id = AddVariableToScope(*labels, token, *string_position - old_position, LabelUnparsedVariableType);
                        UnparsedVariable *unparsed_variable = ((*labels)->variables + variable_id);
                        unparsed_variable->type = LabelUnparsedVariableType;
                        unparsed_variable->address = *byte_count;
                        error_code = 0;
                    }
                    free(token);
                }
                break;
            case DefinePrecursor:
                if(error_code = CreateSourceDefinition(script_tasks, subject_script, &subject_script->values.definitions, &subject_script->values.definition_count, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels)) return PreprocessorDefinitionError;
                break;
            case KeywordPrecursor:
                if(subject_script->rules.privilege_level < MasterPrivilege) return KeywordPrecursorPermissionError;
                if(error_code = CreateSourceDefinition(script_tasks, subject_script, &script_tasks->keywords, &script_tasks->keyword_count, source_length, string_position, byte_count, chunk_count, script_code, script_source, this_scope, scopes, current_scope, labels)) return PreprocessorDefinitionError;
                break;
            case IncludePrecursor:
                
                break;
            case IncludeFilePrecursor:
                break;
            default:
                return InvalidParsingCommandError;
        }
        free(ptoken);
    }

    return error_code;
}

uint64 AllocateScriptMemory(ScriptEnvironment*, Script*, uint64);
int FreeScriptMemory(ScriptEnvironment*, Script*, uint64);
void PrintScriptError(ScriptEnvironment*, Script*, int, byte);

int AddUnparsedVariablesToScript(ScriptEnvironment *script_tasks, Script *subject_script, ScopeManager *scopes, uint64 symbol_count, Symbol *symbol_table) {
    for(int i = 0; i < symbol_count; i++) {
        Symbol new_symbol = *(symbol_table + i);
        byte *name_string = ReadScriptMemoryRange(script_tasks, subject_script, new_symbol.name, (short) new_symbol.length + 1);
        if(!name_string) return 1;
        int var_type = RestoredUnparsedVariableType;
        switch(new_symbol.type) {
            case CustomDataType:
                var_type = TypedefUnparsedVariableType;
        }
        AddVariableToScope(scopes, name_string, (short) new_symbol.length + 1, var_type);
    }
    return 0;
}

int ParseMegaScriptSourceCode(ScriptEnvironment *script_tasks, Script *subject_script) {
    uint64 source_length = strlen(subject_script->values.source) + 1;
    uint64 string_position = 0;
    uint64 byte_count = 0;
    uint64 chunk_count = allocation_chunk_size;
    byte *script_code = malloc(allocation_chunk_size);
    byte *script_source = subject_script->values.source;
    byte opcode = StartScriptOpcode; //for now, this only exists to ensure that functions defined at the beginning of code don't have the same scope position as the 0 scope (the first script scope)
    AddMegaScriptBytes(&opcode, &byte_count, 1, &chunk_count, &script_code);
    if(*script_source == '\0') {
        opcode = EndScriptOpcode;
        AddMegaScriptBytes(&opcode, &byte_count, 1, &chunk_count, &script_code);
        return 0;
    }
    int error_code = 0;

    uint64 current_scope = 0;
    ScopeManager *labels = malloc(sizeof(ScopeManager));
    ScopeManager *scopes = malloc(sizeof(ScopeManager));
    memset(labels, 0, sizeof(ScopeManager));
    memset(scopes, 0, sizeof(ScopeManager));
    labels->variables = malloc(1);
    scopes->variables = malloc(1);
    scopes->id = 0;
    labels->id = 0;

    if(subject_script->values.symbol_count) if(AddUnparsedVariablesToScript(script_tasks, subject_script, scopes, subject_script->values.symbol_count, subject_script->values.symbol_table)) return SymbolMemoryReadFailureError;

    while(string_position < source_length && error_code == 0 && *(script_source + string_position)) {
        ClearWhiteSpace(script_source, &string_position, source_length);
        switch(*(script_source + string_position)) {
            case '#':
                string_position++; //make it label for labels and keyword for keywords
                error_code = ParsePreprocessorCommand(script_tasks, subject_script, source_length, &string_position, &byte_count, &chunk_count, &script_code, script_source, scopes + current_scope, &scopes, &current_scope, &labels); //maybe make it possible to define variables with this, just like assembly
                break;
            default:
                error_code = ParseMegaScriptOperation(script_tasks, subject_script, source_length, &string_position, &byte_count, &chunk_count, &script_code, script_source, scopes + current_scope, &scopes, &current_scope, &labels, 0);
                opcode = EndStatementOpcode;
                AddMegaScriptBytes(&opcode, &byte_count, 1, &chunk_count, &script_code);
        }
        if(error_code) {
            break;
        }
    }

    if(!error_code) {
        opcode = EndScriptOpcode;
        AddMegaScriptBytes(&opcode, &byte_count, 1, &chunk_count, &script_code);
        uint64 offset = AllocateScriptMemory(script_tasks, subject_script, byte_count);

        if(offset) {
            subject_script->values.scope = -1;
            for(int i = 0; i < labels->id; i++) {
                UnparsedVariable *unparsed_variable = labels->variables + i;
                Symbol new_symbol = {0};
                byte *pointer_memory = 0;
                int data_length = 0;
                uint64 address = 0;
                switch(unparsed_variable->type) {
                    case LabelUnparsedVariableType:
                        new_symbol.length = unparsed_variable->length - 1;
                        new_symbol.name = AllocateScriptMemory(script_tasks, subject_script, unparsed_variable->length);
                        new_symbol.scope = -1;
                        pointer_memory = ReadScriptMemoryRange(script_tasks, subject_script, new_symbol.name, unparsed_variable->length);
                        memcpy(pointer_memory, unparsed_variable->name, unparsed_variable->length);
                        data_length = GetDataTypeLength(PointerType);
                        new_symbol.address = AllocateScriptMemory(script_tasks, subject_script, data_length);
                        new_symbol.created_at = 0;
                        new_symbol.type = PointerType;
                        address = unparsed_variable->address + offset;
                        memcpy(script_tasks->memory + new_symbol.address, &address, data_length);
                        CreateLabel(script_tasks, subject_script, new_symbol);
                        break;
                }
            }

            byte *pointer = script_tasks->memory + offset;
            memcpy(pointer, script_code, byte_count);
            if(subject_script->values.program_starting_point == 0) subject_script->values.program_starting_point = offset;
            subject_script->values.program_counter = offset;
            if(CheckSetting(DeveloperByteDisplay)) {
                printf("%d", ReadScriptMemory(script_tasks, subject_script, offset));
                for(int i = 0; i < byte_count - 1; i++) {
                    printf(":%d", ReadScriptMemory(script_tasks, subject_script, offset + i + 1));
                }
                printf("\n");
            }
            if(CheckSetting(DeveloperNotifications)) printf("Script %d parsed successfully. \n", subject_script->id);
        } else {
            error_code = ScriptCodeNotWrittenError;
        }
    } else {
        if(CheckSetting(DeveloperByteDisplay)) {
            printf("%d", *script_code);
            for(int i = 0; i < byte_count - 1; i++) {
                printf(":%d", *(script_code + i + 1));
            }
            printf("\n");
        }
        PrintScriptError(script_tasks, subject_script, error_code, ParsingErrorMode);
    }

    return error_code;
}

