//█████ █   █ █   █ █   █ █████ 
//█     ██  █ █   █ ██ ██ █     
//████  █ █ █ █   █ █ █ █ █████ 
//█     █  ██ █   █ █   █     █ 
//█████ █   █ █████ █   █ █████ 

enum ObjectClasses {
    ScriptClass = -2,
    RootClass
};

enum ScriptStates {
    RunningState,
    WaitingState,
    PausedState,
    StandbyState,
    FinishedState,
    DisabledState,
    ErrorState
};

enum RuleSelection {
    RuleUndefined,
    RuleSelected,
    RuleUnselected
};

enum NumberContainerTypes {
    NullNumberContainerType,
    IntNumberContainerType,
    FloatNumberContainerType,
    DoubleNumberContainerType,
    LongDoubleNumberContainerType,
    PseudoNumberContainerType
};

enum NumberStringTypes {
    NullNumberStringType,
    Base2NumberStringType,
    Base10NumberStringType,
    Base16NumberStringType,
    Base32NumberStringType,
    CustomNumberStringType
};

enum PrecursorTypes {
    NullPrecursor,
    LabelPrecursor,
    IncludePrecursor,
    KeywordPrecursor,
    DefinePrecursor,
    ImportPrecursor,
    PrivilegePrecursor,
    IncludeFilePrecursor
};

enum MegaScriptErrorCodes {
    NoError,
    GenericError,
    InvalidScopeError,
    UndefinedVariableError,
    BitSizeError,
    TakenLabelError,
    PrefixUsedOnOpcodeError,
    MissingVariableNameError,
    MissingCallEndBracketError,
    UnfinishedStringError,
    UndefinedFunctionError,
    UnknownOperatorError,
    InvalidDigitError,
    TemporaryNumberSizeError,
    NotANumberError,
    MissingOperandError,
    MemorySectionOccupiedError,
    NoMemorySectionsError,
    MemorySectionNotFoundError,
    NotEnoughMemoryError,
    MemoryNotOwnedError,
    ScriptCodeNotWrittenError,
    WriteFailureError,
    AddressOutOfBoundsError,
    StackOverflowError,
    ExecutionFaultError,
    MissingParametersError,
    InvalidDataTypeError,
    StandardCallRedefinitionError,
    StandardDataTypeRedefinitionError,
    FloatingPointsModulatedError,
    InvalidOperationError,
    InternalMemoryAllocationError,
    SymbolDefiningError,
    SymbolNotFoundError,
    VariableMemoryAssignmentFailedError,
    StackEmptyError,
    FailedToEnableScriptError,
    AttemptedGetAddressOfImmediateError,
    NullOpcodeExecutionError,
    InvalidJumpDestinationError,
    LabelNotDefinedError,
    LabelParsingError,
    NotAPointerError,
    FunctionArgumentsIncorrectlyPassedError,
    ScriptTimeoutError,
    UnexpectedEndOfScriptError,
    MissingFunctionDefineBracketError,
    TooManyOperatorsError,
    OperationReturnError,
    FunctionReturnError,
    MissingOperationSaveError,
    FunctionCallValuesScrambledError,
    ComplexOperationUsedError,
    NotAFunctionError,
    CouldNotDeleteScopeError,
    CouldNotFindScopeError,
    CouldNotCreateScopeError,
    UnopenedFileError,
    ZeroOperatorError,
    SymbolMemoryReadFailureError,
    LabelMemoryReadFailureError,
    MissingParenthesisError,
    StringGenerationError,
    InvalidArgumentError,
    CouldNotDeleteSymbolError,
    IfStatementFailureError,
    WhileLoopFailureError,
    TooManyStandardCallArgumentsError,
    TooFewStandardCallArgumentsError,
    BytecodeNotFoundError,
    StandardCallPrivilegeError,
    ForLoopSaveError,
    ContinueOutsideOfLoopError,
    CannotGoHigherError,
    BreakOutsideOfLoopError,
    InvalidStackDataInReturnError,
    IncrementedNonSymbolError,
    DecrementedNonSymbolError,
    InvalidOperatorTypeError,
    AssignedNonSymbolError,
    InvalidParsingCommandError,
    StandardCallIdNotFoundError,
    StandardCallIdDoesNotExistError,
    MissingStandardCallbackError,
    AttemptedSymbolSettingsChangeOnNonSymbolError,
    CouldNotFindTheSymbolError,
    CouldNotReadMemoryError,
    CouldNotWriteMemoryError,
    AttemptedModificationOfConstantError,
    LambdaFunctionDefiningError,
    CastedWithNonDataTypeError,
    EnumDefiningError,
    FunctionCallInsideEnumError,
    EnumBracketMismatchError,
    EnumElementBytecodeOutsideError,
    NonExistentEnumMemberError,
    InvalidElementTypeError,
    EnumListReadFailureError,
    KeywordPrecursorPermissionError,
    MissingStackDataError,
    UnsupportedStackDataTypeError,
    ScriptIsReadOnlyError,
    DataNotAStringError,
    MemoryReadError,
    UnableToCreateStringError,
    ScriptNameNotFoundError,
    RequiredStandardScriptError,
    PreprocessorDefinitionError,
    NewMissingDataTypeError,
    TooManyBoolElementsError,
    UnsupportedNewDataTypeError,
    DebrisNotFoundError,
    BoolPointerError,
    ArraySizeInitializationError,
    InvalidExecutionBytecodeError,
    ArrayInitializerMemoryReadError,
    ComplexOperationPerformedInArrayInitializerError,
    MissingEndingArrayInitializerBracketError,
    CPointerZeroingError,
    AttemptedArrayDataSizeZeroError,
    ArrayMemoryAllocationFailedError,
    ArrayPointerAssigningError,
    PointerDimensionZeroError,
    FailedToReadPointerError,
    FailedToFindDebrisInDereferenceError,
    NotASymbolOrDebrisError,
    FailedToFindDebrisByIdError,
    FailedToCreateDebrisError,
    FailedToAllocateMemoryError,
    MissingArrayIndexClosingBracketError,
    NotAnArrayError,
    NotACLikePointerError,
    StoppedAtCommaError,
    StoppedAtCommaWithBracketError,
    ImproperUsageOfCommaError,
    MultiDimensionalArraysNotYetAddedError,
    NotAStructError,
    StructListCouldNotBeFoundError,
    RecursiveStructListPointerNotFoundError,
    FailedToPopStructError,
    ImproperUsageOfNewError,
    FailedToAllocateStructListMemoryError,
    FailedToSetTheStructMemberCountError,
    FailedToSetTheStructElementsPointerError,
    FailedToSetTheStructDataTypesPointerError,
    FailedToSetTheStructAuxilaryPointerError,
    InitializedArrayInStructError,
    StructureAutoIncrementDesyncedError,
    TypedefNotFoundError,
    StructElementNotFoundError,
    FailedToGetStructSizeError,
    CouldNotFindStructListPointerError,
    FailedToGetStructDataError,
    NotABasicDataTypeError,
    InvalidObjectClassError,
    ObjectDoesNotExistError,
    AttemptToAssignNonStructToStructError,
    MissingClosingParenthesisOnCallbackError,
    NotAnObjectCallbackError,
    ObjectCallbackError,
    EnvironmentRootNotFoundError,
    AttemptedToSetObjectParentToNonObjectError,
    CouldNotFindClassTypedefError,
    PrivilegeLevelError,
    FailedToPopStringError,
    FailedToFindClassError,
    Unsupported8BitIntError,
    Unsupported16BitIntError,
    Unsupported32BitIntError,
    Unsupported64BitIntError,
    Unsupported128BitIntError,
    Unsupported8BitFloatError,
    Unsupported16BitFloatError,
    Unsupported32BitFloatError,
    Unsupported64BitFloatError,
    Unsupported128BitFloatError,
    MegaScript32BitVersionError,
    ObjectInStructError,
    AttemptedToAssignNonObjectToObjectError,
    ListPointerNotFoundError,
    ListPointerDoesNotMatchError,
    AttemptedToAllocateZeroError,
    AttemptedToGetAddressOfObjectError,
    InvalidSyntaxError,
    InvalidOperatorSyntaxError,
    CommasCannotBeUsedHereError,
    FailedToCreateASMFunctionError
};

enum MegaScriptPrivilegeLevels {
    RestrictedPrivilege,
    LowPrivilege,
    MediumPrivilege,
    HighPrivilege,
    SuperPrivilege,
    MasterPrivilege,
    DebugPrivilege,
    LockedPrivilege = 255
};

enum MegaScriptErrorPrintModes {
    ExecutionErrorMode,
    ParsingErrorMode,
    EnvironmentErrorMode
};

enum ScriptTypes {
    StandardScriptType,
    ModuleScriptType,
    LibraryScriptType
};

enum PrecedenceDirection {
    LeftToRight,
    RightToLeft
};

enum OperatorTypes {
    NormalOperatorType,
    PrefixOperatorType,
    SuffixOperatorType,
    OpeningBracketOperatorType,
    ClosingBracketOperatorType
};

enum BytecodeTypes {
    StandardCallBytecodeType,
    OperatorBytecodeType,
    SyntaxBytecodeType
};

enum TextEncodingTypes {
    AsciiTextType
};

enum UnparsedVariableTypes {
    RestoredUnparsedVariableType,
    LabelUnparsedVariableType,
    TypedefUnparsedVariableType
};

enum OSTypes {
    UndefinedOS,
    WindowsOS,
    LinuxOS,
    BSDOS,
    MacOS,
    TempleOS
};

enum ObjectMethodTypes {
    ElementMethodType,
    FunctionMethodType
};