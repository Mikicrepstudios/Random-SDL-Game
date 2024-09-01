//█   █  ███  ████  █████  ███  ████  █     █████ █████ 
//█   █ █   █ █   █   █   █   █ █   █ █     █     █     
// █ █  █████ ████    █   █████ ████  █     ████  █████ 
// █ █  █   █ █   █   █   █   █ █   █ █     █         █ 
//  █   █   █ █   █ █████ █   █ ████  █████ █████ █████ 

//convenient values
#define MegascriptMax64Bit 18446744073709551615ULL

//bool settings
#define DynamicIntegerResizing 1U
#define DynamicFloatResizing 2U
#define DeveloperByteDisplay 4U
#define DynamicStackResizing 8U
#define DeveloperNotifications 16U

//megascript technical configuration
#define megascript_bool_settings 2U //set to the number that your bool settings make added together
#define megascript_default_float_type 3
#define default_script_timeout_megascript 60 //60 seconds. 0.001 for 1 millisecond
#define default_min_memory_megascript 1048576 //1 Megabyte //make sure to make a passthrough that lets users see these from megascript
#define default_max_memory_megascript 268435456 //256 Megabytes
#define default_reserve_memory_megascript 65536 //64 Kilobytes reserved for user (Can be read and written to by all scripts if chosen in environment settings)
#define allocation_chunk_size 512
#define megascript_memory_search_gap 32 //This defines the amount of bytes that the memory searcher skips when searching for memory section address matches (Do not worry, as it skips only until the result pointer is higher than the desired pointer. It then back tracks until it either finds the exact address, or fails.)
#define megascript_stack_size 8192 //todo: maybe make a single number with bit flags for bool settings
#define megascript_operator_storage_size 1024
#define megascript_symbol_hash_frequency 1024

//megascript constants (DO NOT CHANGE)
#define bit_mode 64

//operation save settings
#define HasReturn 1U
#define ResumePrevious 2U
#define PrimitiveOnly 4U
#define HasFunctionCall 8U
#define IsCallReturn 16U
#define IsFunctionArgument 32U
#define IsLoop 64U
#define StopAtComma 128U

//operation parsing settings
#define StopAtTernary 1U
#define LastWasUnion 2U
#define IsInsideEnum 4U
#define IsInsideStruct 8U
#define IsArrayInitializer 16U
#define StopAtTopCommas 32U

//standard call parsing settings
#define CallHasBracket 1U
#define CallEntersScope 2U
#define SkipStopStandardCallBytecode 4U
#define SnipsParenthesis 8U

//print call settings
#define MakeNewLine 1U

//symbol settings
#define IsConstant 1U
#define AutoFreeMemory 2U
#define IsCLike 4U
#define PointsToPointer 8U
#define IsArray 16U
#define HasGeniunePointer 32U
#define DoNotFreeSymbolAddress 64U
#define AutoFreePointer 128U

//script settings
#define CanAccessForeignMemory 1U
#define NameWasMalloced 2U

//script rules
#define MemoryProtectionOn 1U
#define CanCreateObjects 2U
#define CanModifyObjects 4U
#define CanDefineObjects 8U
#define CanCreateScripts 16U
#define CanIgnoreMemoryProtection 32U
#define IsReadOnly 64U

//stack data settings
#define StackDataWasMallocated 1U

//environment settings
#define ScriptsInheritFromMaster 1U
#define StringsPointToCode 2U
#define EnvironmentUsesRealAddresses 4U

//stack data expand options
#define DeleteDebrisInStackData 1U

//bool container settings
#define BoolHasGeniunePointer 1U

//====data types begin====
#define VoidType (unsigned char) 0U//maybe add an event data type
#define NullType (unsigned char) 1U
#define Int8Type (unsigned char) 2U
#define Int16Type (unsigned char) 3U
#define Int32Type (unsigned char) 4U
#define Int64Type (unsigned char) 5U
#define UnsignedInt8Type (unsigned char) 6U
#define UnsignedInt16Type (unsigned char) 7U
#define UnsignedInt32Type (unsigned char) 8U
#define UnsignedInt64Type (unsigned char) 9U
#define FloatType (unsigned char) 10U
#define DoubleType (unsigned char) 11U
#define LongDoubleType (unsigned char) 12U
#define PseudoNumberType (unsigned char) 13U
#define StructType (unsigned char) 14U
#define ClassType (unsigned char) 15U
#define FunctionType (unsigned char) 16U
#define ListType (unsigned char) 17U
#define DynamicArrayType (unsigned char) 18U
#define BoolType (unsigned char) 19U //unused
#define ObjectType (unsigned char) 20U
#define PointerType (unsigned char) 21U
#define SymbolType (unsigned char) 22U
#define LabelType (unsigned char) 23U //unused
#define CustomSymbolType (unsigned char) 24U //unused
#define StringType (unsigned char) 25U
#define CharType (unsigned char) 26U
#define GenericType (unsigned char) 27U
#define OperationType (unsigned char) 28U
#define CPointerType (unsigned char) 29U
#define DataType (unsigned char) 30U
#define ForLoopSaveType (unsigned char) 31U
#define WhileLoopSaveType (unsigned char) 32U //unused
#define SwitchStatementSaveType (unsigned char) 33U
#define TemporarySymbolType (unsigned char) 34U
#define ElementType (unsigned char) 35U
#define EnumType (unsigned char) 36U
#define OldListType (unsigned char) 37U //unused
#define EnumStackSaveType (unsigned char) 38U
#define AutoDeleteType (unsigned char) 39U
#define Bool8Type (unsigned char) 40U
#define Bool16Type (unsigned char) 41U
#define Bool32Type (unsigned char) 42U
#define Bool64Type (unsigned char) 43U
#define NewDataType (unsigned char) 44U
#define Bool8ContainerType (unsigned char) 45U
#define Bool16ContainerType (unsigned char) 46U
#define Bool32ContainerType (unsigned char) 47U
#define Bool64ContainerType (unsigned char) 48U
#define ObjectClassType (unsigned char) 49U
#define ObjectCallbackType (unsigned char) 50U
#define CFunctionType (unsigned char) 51U

#define CustomDataType (unsigned char) 255U
//====data types end====

//====bytecodes begin====
#define NullOpcode (unsigned char) 0U
#define FunctionOpcode (unsigned char) 1U
#define IfOpcode (unsigned char) 2U
#define ForOpcode (unsigned char) 3U
#define WhileOpcode (unsigned char) 4U
#define StopOpcode (unsigned char) 5U
#define JumpOpcode (unsigned char) 6U
#define ReturnOpcode (unsigned char) 7U
#define SizeofOpcode (unsigned char) 8U
#define ClassOpcode (unsigned char) 9U
#define NewAliasOpcode (unsigned char) 10U
#define EnumOpcode (unsigned char) 11U
#define SwitchOpcode (unsigned char) 12U
#define WaitOpcode (unsigned char) 13U
#define PrintOpcode (unsigned char) 14U
#define BoolElementOpcode (unsigned char) 15U //unused
#define DataTypeOpcode (unsigned char) 16U
#define CustomDataTypeOpcode (unsigned char) 17U
#define VariableOpcode (unsigned char) 18U
#define CreateVariableOpcode (unsigned char) 19U
#define ConstructSymbolOpcode (unsigned char) 20U
#define ArraySizeOpcode (unsigned char) 21U //unused
#define SignedIntegerOpcode (unsigned char) 22U
#define StringOpcode (unsigned char) 23U
#define CharacterOpcode (unsigned char) 24U
#define IntegerOpcode (unsigned char) 25U
#define FloatOpcode (unsigned char) 26U
#define ElseOpcode (unsigned char) 27U
#define ElseIfOpcode (unsigned char) 28U
#define EnumElementOpcode (unsigned char) 29U
#define BreakOpcode (unsigned char) 30U
#define ContinueOpcode (unsigned char) 31U
#define EndStatementOpcode (unsigned char) 32U
#define EndScriptOpcode (unsigned char) 33U
#define StartScriptOpcode (unsigned char) 34U
#define CancelOpcode (unsigned char) 35U
#define ElementCallbackOperator (unsigned char) 36U
#define NewOpcode (unsigned char) 37U
#define ArrayAccessOpcode (unsigned char) 38U //unused

//left to right
#define CallOperator (unsigned char) 39U
#define CallEndOperator (unsigned char) 40U
#define IncrementOperator (unsigned char) 41U
#define DecrementOperator (unsigned char) 42U
#define ArrayOperator (unsigned char) 43U
#define ArrayEndOperator (unsigned char) 44U
#define DynamicOperator (unsigned char) 45U
#define DynamicEndOperator (unsigned char) 46U
#define ConnectorOperator (unsigned char) 47U
#define PointerConnectorOperator (unsigned char) 48U

//right to left
#define PrefixIncrementOperator (unsigned char) 49U
#define PrefixDecrementOperator (unsigned char) 50U
#define UnaryPlusOperator (unsigned char) 51U
#define UnaryMinusOperator (unsigned char) 52U
#define LogicalNotOperator (unsigned char) 53U
#define BitwiseNotOperator (unsigned char) 54U
#define TypeCastOperator (unsigned char) 55U //unused
#define DereferenceOperator (unsigned char) 56U
#define AddressOfOperator (unsigned char) 57U
#define ArrayIndexOperator (unsigned char) 58U

//left to right
#define MultiplyOperator (unsigned char) 59U
#define DivideOperator (unsigned char) 60U
#define RemainderOperator (unsigned char) 61U

#define AddOperator (unsigned char) 62U
#define SubtractOperator (unsigned char) 63U

#define BitwiseShiftLeftOperator (unsigned char) 64U
#define BitwiseShiftRightOperator (unsigned char) 65U

#define LessThanOperator (unsigned char) 66U
#define LessThanOrEqualToOperator (unsigned char) 67U
#define GreaterThanOperator (unsigned char) 68U
#define GreaterThanOrEqualToOperator (unsigned char) 69U

#define EqualOperator (unsigned char) 70U
#define NotEqualOperator (unsigned char) 71U

#define BitwiseAndOperator (unsigned char) 72U

#define BitwiseXorOperator (unsigned char) 73U

#define BitwiseOrOperator (unsigned char) 74U

#define LogicalAndOperator (unsigned char) 75U

#define LogicalOrOperator (unsigned char) 76U

#define IntelligentAndOperator 77U

#define IntelligentOrOperator 78U

#define TernaryOneOperator (unsigned char) 79U
#define TernaryTwoOperator (unsigned char) 80U

//right to left
#define AssignOperator (unsigned char) 81U
#define AddAssignOperator (unsigned char) 82U
#define SubtractAssignOperator (unsigned char) 83U
#define MultiplyAssignOperator (unsigned char) 84U
#define DivideAssignOperator (unsigned char) 85U
#define RemainderAssignOperator (unsigned char) 86U
#define ShiftLeftAssignOperator (unsigned char) 87U
#define ShiftRightAssignOperator (unsigned char) 88U
#define AndAssignOperator (unsigned char) 89U
#define XorAssignOperator (unsigned char) 90U
#define OrAssignOperator (unsigned char) 91U

//left to right
#define CommaOperator (unsigned char) 92U

#define PushOpcode (unsigned char) 93U //unused
#define PopOpcode (unsigned char) 94U //unused
#define SpaceofOpcode (unsigned char) 95U
#define DeleteOpcode (unsigned char) 96U //unused
#define RunOpcode (unsigned char) 97U //unused
#define LoadOpcode (unsigned char) 98U
#define ElementOpcode (unsigned char) 99U
#define KeywordOpcode (unsigned char) 100U //unused
#define CallOpcode (unsigned char) 101U //unused
#define PassthroughOpcode (unsigned char) 102U //unused
#define StandardCallOpcode (unsigned char) 103U
#define LambdaFunctionOpcode (unsigned char) 104U
#define StopStandardCallOpcode (unsigned char) 105U

#define OrientedDereferenceOperator (unsigned char) 106U //unused
#define CastOperator (unsigned char) 107U

//extended opcodes
#define ExtendedOpcode (unsigned char) 255U
//====bytecodes end====

//====symbol constructor codes begin====
#define NullSymbolConstructorCode 0U
#define ArrayInitializerSymbolConstructorCode 1U
#define PointerMarkerSymbolConstructorCode 2U
#define EndSymbolConstructorCode 3U
#define CreateStructElementSymbolConstructorCode 4U
#define CreateVariableSymbolConstructorCode 19U //make sure it's the same as CreateVariableOpcode
//====symbol constructor codes end====

//megascript
static unsigned int script_id_megascript = 0;
static int current_object_id = 0;
static short megascript_memory_protection = 1;
static ScriptEnvironment *master_script_container_megascript;
//static const long long unsigned int MegascriptMax64Bit = 18446744073709551615ULL;
static long long unsigned int megascript_parser_line = 0;
static long long unsigned int megascript_parser_column = 0;
static char default_megascript_initializer_file[] = "initialize.mega";

#ifdef _WIN32
static unsigned char current_os = WindowsOS;
#elif linux
static unsigned char current_os = LinuxOS;
#else
static unsigned char current_os = UndefinedOS;
#endif

//megascript data types (unlisted opcodes will default to restricted privilege and zero arguments)
#define bytecode_information_count 14
static BytecodeInformation bytecode_information[] = {
    {LoadOpcode, MasterPrivilege, 1},
    {KeywordOpcode, MasterPrivilege, 1},
    {CallOpcode, SuperPrivilege, 1},
    {RunOpcode, SuperPrivilege, 1},
    {PushOpcode, HighPrivilege, 1},
    {PopOpcode, HighPrivilege, 1},
    {WaitOpcode, RestrictedPrivilege, 1},
    {PrintOpcode, RestrictedPrivilege, 1},
    {WhileOpcode, RestrictedPrivilege, 1},
    {IfOpcode, RestrictedPrivilege, 1}, //This is 1 and not three because the first argument of a for loop is the only static one.
    {SizeofOpcode, RestrictedPrivilege, 1},
    {SpaceofOpcode, RestrictedPrivilege, 1},
    {ForOpcode, RestrictedPrivilege, 3},
    {ReturnOpcode, RestrictedPrivilege, 1}
};

//megascript standard calls
#define megascript_standard_call_count 32
static Word megascript_default_standard_calls[] = {
    {"function", FunctionOpcode},
    {"stop", StopOpcode},
    {"jump", JumpOpcode},
    {"return", ReturnOpcode},
    {"if", IfOpcode},
    {"for", ForOpcode},
    {"while", WhileOpcode},
    {"sizeof", SizeofOpcode},
    {"elseif", ElseIfOpcode},
    {"else", ElseOpcode},
    {"class", ClassOpcode},
    {"switch", SwitchOpcode},
    {"wait", WaitOpcode},
    {"print", PrintOpcode},
    {"break", BreakOpcode},
    {"continue", ContinueOpcode},
    {"nevermind", CancelOpcode},
    {"cancel", CancelOpcode},
    {"spaceof", SpaceofOpcode},
    {"load", LoadOpcode},
    {"exit", EndScriptOpcode},
    {"new", NewOpcode},
    {"struct", NewAliasOpcode},
    {"bool8", NewAliasOpcode},
    {"bool16", NewAliasOpcode},
    {"bool32", NewAliasOpcode},
    {"bool64", NewAliasOpcode},
    {"flags8", NewAliasOpcode},
    {"flags16", NewAliasOpcode},
    {"flags32", NewAliasOpcode},
    {"flags64", NewAliasOpcode},
    {"enum", NewAliasOpcode}
};

//megascript data types
#define megascript_data_type_count 51
static Word megascript_default_data_types[] = {
    {"void", NullType},
    {"char", CharType},
    {"byte", UnsignedInt8Type},
    {"pointer", PointerType},
    {"string", PointerType},
    {"int", Int32Type},
    {"int8", Int8Type},
    {"int16", Int16Type},
    {"int32", Int32Type},
    {"int64", Int64Type},
    {"i8", Int8Type},
    {"i16", Int16Type},
    {"i32", Int32Type},
    {"i64", Int64Type},
    {"u8", UnsignedInt8Type},
    {"u16", UnsignedInt16Type},
    {"u32", UnsignedInt32Type},
    {"u64", UnsignedInt64Type},
    {"uint8", UnsignedInt8Type},
    {"uint16", UnsignedInt16Type},
    {"uint32", UnsignedInt32Type},
    {"uint64", UnsignedInt64Type},
    {"unsigned", UnsignedInt32Type},
    {"unsigned8", UnsignedInt8Type},
    {"unsigned16", UnsignedInt16Type},
    {"unsigned32", UnsignedInt32Type},
    {"unsigned64", UnsignedInt64Type},
    {"float", FloatType},
    {"double", DoubleType},
    {"long", LongDoubleType},
    {"float32", FloatType},
    {"float64", DoubleType},
    {"float128", LongDoubleType},
    {"f32", FloatType},
    {"f64", DoubleType},
    {"f128", LongDoubleType},
    {"number", PseudoNumberType},
    {"dynamic", DynamicArrayType},
    {"bool", UnsignedInt8Type},
    {"object", ObjectType},
    {"generic", GenericType},
    {"bool8", Bool8Type},
    {"bool16", Bool16Type},
    {"bool32", Bool32Type},
    {"bool64", Bool64Type},
    {"enum", EnumType},
    {"struct", StructType},
    {"flags8", Bool8Type},
    {"flags16", Bool16Type},
    {"flags32", Bool32Type},
    {"flags64", Bool64Type}
};

#define megascript_precursors_count 6
static Word megascript_default_precursors[] = {
    {"label", LabelPrecursor},
    {"include", IncludePrecursor},
    {"define", DefinePrecursor},
    {"keyword", KeywordPrecursor},
    {"import", ImportPrecursor},
    {"include_file", IncludeFilePrecursor}
};

#define operator_words_count 3
static Word operator_words[] = {
    {"and", IntelligentAndOperator},
    {"or", IntelligentOrOperator},
    {"not", LogicalNotOperator}
};

#define precedence_groups_count 19 //Higher precedence has a lower index value. This means that the first group in the array is the highest precedence. (This will automatically correct itself when the operator array is populated during the initialization. A higher precedence number will actually mean higher precedence.)
static PrecedenceGroup precedence_groups[] = {
    {
        LeftToRight, //---->>>>
        12,
        (unsigned char[]) {
            CallOperator,
            CallEndOperator,
            IncrementOperator,
            DecrementOperator,
            ArrayOperator,
            ArrayEndOperator,
            DynamicOperator,
            DynamicEndOperator,
            ConnectorOperator,
            PointerConnectorOperator,
            ArrayIndexOperator,
            ElementCallbackOperator
        }
    },
    {
        RightToLeft, //<<<<----
        9,
        (unsigned char[]) {
            PrefixIncrementOperator,
            PrefixDecrementOperator,
            UnaryPlusOperator,
            UnaryMinusOperator,
            LogicalNotOperator,
            BitwiseNotOperator,
            DereferenceOperator,
            AddressOfOperator,
            CastOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            MultiplyOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            DivideOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            RemainderOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            AddOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            SubtractOperator
        }
    },
    {
        LeftToRight, //---->>>>
        2,
        (unsigned char[]) {
            BitwiseShiftLeftOperator,
            BitwiseShiftRightOperator
        }
    },
    {
        LeftToRight, //---->>>>
        4,
        (unsigned char[]) {
            LessThanOperator,
            LessThanOrEqualToOperator,
            GreaterThanOperator,
            GreaterThanOrEqualToOperator
        }
    },
    {
        LeftToRight, //---->>>>
        2,
        (unsigned char[]) {
            EqualOperator,
            NotEqualOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            BitwiseAndOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            BitwiseXorOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            BitwiseOrOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            LogicalAndOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            LogicalOrOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            IntelligentAndOperator
        }
    },
    {
        LeftToRight, //---->>>>
        2,
        (unsigned char[]) {
            TernaryOneOperator,
            TernaryTwoOperator
        }
    },
    {
        RightToLeft, //<<<<----
        11,
        (unsigned char[]) {
            AssignOperator,
            AddAssignOperator,
            SubtractAssignOperator,
            MultiplyAssignOperator,
            DivideAssignOperator,
            RemainderAssignOperator,
            ShiftLeftAssignOperator,
            ShiftRightAssignOperator,
            AndAssignOperator,
            XorAssignOperator,
            OrAssignOperator
        }
    },
    {
        LeftToRight, //---->>>>
        1,
        (unsigned char[]) {
            CommaOperator
        }
    }
};

#define operator_configuration_count 56
static KeyValue operator_configuration[] = {
    {CallOperator, OpeningBracketOperatorType},
    {CallEndOperator, ClosingBracketOperatorType},
    {IncrementOperator, SuffixOperatorType},
    {DecrementOperator, SuffixOperatorType},
    {ArrayOperator, OpeningBracketOperatorType},
    {ArrayEndOperator, ClosingBracketOperatorType},
    {DynamicOperator, OpeningBracketOperatorType},
    {DynamicEndOperator, ClosingBracketOperatorType},
    {ConnectorOperator, NormalOperatorType},
    {PointerConnectorOperator, NormalOperatorType},
    {PrefixIncrementOperator, PrefixOperatorType},
    {PrefixDecrementOperator, PrefixOperatorType},
    {UnaryPlusOperator, PrefixOperatorType},
    {UnaryMinusOperator, PrefixOperatorType},
    {LogicalNotOperator, PrefixOperatorType},
    {BitwiseNotOperator, PrefixOperatorType},
    {TypeCastOperator, PrefixOperatorType},
    {DereferenceOperator, PrefixOperatorType},
    {AddressOfOperator, PrefixOperatorType},
    {ArrayIndexOperator, NormalOperatorType},
    {MultiplyOperator, NormalOperatorType},
    {DivideOperator, NormalOperatorType},
    {RemainderOperator, NormalOperatorType},
    {AddOperator, NormalOperatorType},
    {SubtractOperator, NormalOperatorType},
    {BitwiseShiftLeftOperator, NormalOperatorType},
    {BitwiseShiftRightOperator, NormalOperatorType},
    {LessThanOperator, NormalOperatorType},
    {LessThanOrEqualToOperator, NormalOperatorType},
    {GreaterThanOperator, NormalOperatorType},
    {GreaterThanOrEqualToOperator, NormalOperatorType},
    {EqualOperator, NormalOperatorType},
    {NotEqualOperator, NormalOperatorType},
    {BitwiseAndOperator, NormalOperatorType},
    {BitwiseXorOperator, NormalOperatorType},
    {BitwiseOrOperator, NormalOperatorType},
    {LogicalAndOperator, NormalOperatorType},
    {LogicalOrOperator, NormalOperatorType},
    {IntelligentAndOperator, NormalOperatorType},
    {IntelligentOrOperator, NormalOperatorType},
    {TernaryOneOperator, NormalOperatorType},
    {TernaryTwoOperator, NormalOperatorType},
    {AssignOperator, NormalOperatorType},
    {AddAssignOperator, NormalOperatorType},
    {SubtractAssignOperator, NormalOperatorType},
    {MultiplyAssignOperator, NormalOperatorType},
    {DivideAssignOperator, NormalOperatorType},
    {RemainderAssignOperator, NormalOperatorType},
    {ShiftLeftAssignOperator, NormalOperatorType},
    {ShiftRightAssignOperator, NormalOperatorType},
    {AndAssignOperator, NormalOperatorType},
    {XorAssignOperator, NormalOperatorType},
    {OrAssignOperator, NormalOperatorType},
    {CommaOperator, NormalOperatorType},
    {CastOperator, NormalOperatorType},
    {ElementCallbackOperator, NormalOperatorType}
};

#define syntax_bytecodes_count 22
static unsigned char syntax_bytecodes[] = {
    FunctionOpcode,
    NullOpcode,
    ClassOpcode,
    NewAliasOpcode,
    EnumOpcode,
    VariableOpcode,
    CreateVariableOpcode,
    SignedIntegerOpcode,
    StringOpcode,
    CharacterOpcode,
    IntegerOpcode,
    FloatOpcode,
    EndStatementOpcode,
    EndScriptOpcode,
    StartScriptOpcode,
    LambdaFunctionOpcode,
    DataTypeOpcode,
    EnumElementOpcode,
    ElementOpcode,
    NewOpcode,
    ConstructSymbolOpcode,
    StopStandardCallOpcode
};

//typedef
typedef short unsigned int uint16;
typedef short int int16;
typedef unsigned int uint32;
typedef int int32;
typedef long long unsigned int uint64;
typedef long long int int64;
typedef unsigned char byte;
typedef float float32;
typedef double float64;
typedef long double float128;