grammar kiki;

program: sourceElements? EOF;

sourceElements: sourceElement+;

sourceElement: statement;

statement
    : block
    | functionDeclaration
    | expressionStatement
    | variableStatement
    | importStatement
    | makeupStatement;

block: '{' statementList? '}';

statementList: statement+;

functionDeclaration
    : Function Identifier '(' formalParameterList? ')' '{' functionBody '}';

formalParameterList
    : formalParameterArg (',' formalParameterArg)* (
        ',' lastFormalParameterArg
    )?
    | lastFormalParameterArg;

formalParameterArg: Identifier ('=' singleExpression)?;

lastFormalParameterArg: Ellipsis Identifier;

functionBody: sourceElements?;

singleExpression
    : Function Identifier? '(' formalParameterList? ')' '{' functionBody '}' # FunctionExpression
    | singleExpression arguments                                             # ArgumentsExpression
    | singleExpression '[' expressionSequence ']'                            # MemberIndexExpression
    | singleExpression '.' Identifier                                        # MemberDotExpression
    | singleExpression '++'                                                  # PostIncrementExpression
    | singleExpression '--'                                                  # PostDecreaseExpression
    | Delete singleExpression                                                # DeleteExpression
    | Void singleExpression                                                  # VoidExpression
    | Typeof singleExpression                                                # TypeofExpression
    | '++' singleExpression                                                  # PreIncrementExpression
    | '--' singleExpression                                                  # PreDecreaseExpression
    | '+' singleExpression                                                   # UnaryPlusExpression
    | '-' singleExpression                                                   # UnaryMinusExpression
    | '~' singleExpression                                                   # BitNotExpression
    | '!' singleExpression                                                   # NotExpression
    | singleExpression ('*' | '/' | '%') singleExpression                    # MultiplicativeExpression
    | singleExpression ('+' | '-') singleExpression                          # AdditiveExpression
    | singleExpression ('<<' | '>>') singleExpression                        # BitShiftExpression
    | singleExpression ('<' | '>' | '<=' | '>=') singleExpression            # RelationalExpression
    | singleExpression Is singleExpression                                   # IsExpression
    | singleExpression In singleExpression                                   # InExpression
    | singleExpression ('==' | '!=' | '===' | '!==') singleExpression        # EqualityExpression
    | singleExpression '&' singleExpression                                  # BitAndExpression
    | singleExpression '^' singleExpression                                  # BitXOrExpression
    | singleExpression '|' singleExpression                                  # BitOrExpression
    | singleExpression '&&' singleExpression                                 # LogicalAndExpression
    | singleExpression '||' singleExpression                                 # LogicalOrExpression
    | singleExpression '?' singleExpression ':' singleExpression             # TernaryExpression
    | singleExpression '=' singleExpression                                  # AssignmentExpression
    | This                                                                   # ThisExpression
    | Identifier                                                             # IdentifierExpression
    | literal                                                                # LiteralExpression
    | singleExpression TemplateStringLiteral                                 # TemplateStringExpression;

expressionStatement: expressionSequence eos?;

expressionSequence: singleExpression (',' singleExpression)*;

variableStatement: varModifier variableDeclarationList eos;

variableDeclarationList
    : variableDeclaration (',' variableDeclaration)*;

variableDeclaration
    : (Identifier) (':' type)? ('=' singleExpression)?;

arguments: '(' ( singleExpression (',' singleExpression)*)? ')';

importStatement: Import Identifier (Dot Identifier)* eos;

varModifier: Var | Let | Const;

type: Identifier;

MultiLineComment: '/*' .*? '*/' -> channel(HIDDEN);
SingleLineComment
    : '//' ~[\r\n\u2028\u2029]* -> channel(HIDDEN);

OpenBracket:                '[';
CloseBracket:               ']';
OpenParen:                  '(';
CloseParen:                 ')';
OpenBrace:                  '{';
CloseBrace:                 '}';
SemiColon:                  ';';
Comma:                      ',';
Assign:                     '=';
QuestionMark:               '?';
Colon:                      ':';
Ellipsis:                   '...';
Dot:                        '.';
PlusPlus:                   '++';
MinusMinus:                 '--';
Plus:                       '+';
Minus:                      '-';
BitNot:                     '~';
Not:                        '!';
Multiply:                   '*';
Divide:                     '/';
Modulus:                    '%';
RightShiftArithmetic:       '>>';
LeftShiftArithmetic:        '<<';
LessThan:                   '<';
MoreThan:                   '>';
LessThanEquals:             '<=';
GreaterThanEquals:          '>=';
Equals_:                    '==';
NotEquals:                  '!=';
BitAnd:                     '&';
BitXOr:                     '^';
BitOr:                      '|';
And:                        '&&';
Or:                         '||';
MultiplyAssign:             '*=';
DivideAssign:               '/=';
ModulusAssign:              '%=';
PlusAssign:                 '+=';
MinusAssign:                '-=';
LeftShiftArithmeticAssign:  '<<=';
RightShiftArithmeticAssign: '>>=';
BitAndAssign:               '&=';
BitXorAssign:               '^=';
BitOrAssign:                '|=';
ARROW:                      '=>';

/// Keywords

Let:      'let';
Var:      'var';
Const:    'const';
If:       'if';
Else:     'else';
Do:       'do';
While:    'while';
For:      'for';
In:       'in';
Continue: 'continue';
Break:    'break';
Switch:   'switch';
Case:     'case';
Default:  'default';
Is:       'is';
Typeof:   'typeof';
Void:     'void';
New:      'new';
Delete:   'delete';
Class:    'class';
Function: 'function';
Return:   'return';
This:     'this';
With:     'with';
Throw:    'throw';
Try:      'try';
Catch:    'catch';
Finally:  'finally';
Module:   'module';
Export:   'export';
Import:   'import';
Yield:    'yield';

/// Numeric Literals

DecimalLiteral
    : DecimalIntegerLiteral '.' [0-9]* ExponentPart?
    | '.' [0-9]+ ExponentPart?
    | DecimalIntegerLiteral ExponentPart?;

fragment DecimalIntegerLiteral: '0' | [1-9] [0-9]*;

fragment ExponentPart: [eE] [+-]? [0-9]+;

HexIntegerLiteral:    '0' [xX] HexDigit+;
OctalIntegerLiteral:  '0' [0-7]+;
OctalIntegerLiteral2: '0' [oO] [0-7]+;
BinaryIntegerLiteral: '0' [bB] [01]+;

fragment HexDigit: [0-9a-fA-F];

Identifier: [_$@a-zA-Z] [a-zA-Z0-9_]*;

literal
    : NullLiteral
    | BooleanLiteral
    | StringLiteral
    | TemplateStringLiteral
    | numericLiteral;

NullLiteral: 'null';

BooleanLiteral: 'true' | 'false';

StringLiteral
    : (
        '"' DoubleStringCharacter* '"'
        | '\'' SingleStringCharacter* '\''
    );

TemplateStringLiteral: '`' ('\\`' | ~'`')* '`';

numericLiteral
    : DecimalLiteral
    | HexIntegerLiteral
    | OctalIntegerLiteral
    | OctalIntegerLiteral2
    | BinaryIntegerLiteral;

WhiteSpaces: [\t\u000B\u000C\u0020\u00A0]+ -> channel(HIDDEN);

LineTerminator: [\r\n\u2028\u2029] -> channel(HIDDEN);

fragment EscapeSequence
    : CharacterEscapeSequence
    | '0' // no digit ahead! TODO
    | HexEscapeSequence
    | UnicodeEscapeSequence
    | ExtendedUnicodeEscapeSequence;

fragment CharacterEscapeSequence
    : SingleEscapeCharacter
    | NonEscapeCharacter;
fragment HexEscapeSequence: 'x' HexDigit HexDigit;
fragment UnicodeEscapeSequence
    : 'u' HexDigit HexDigit HexDigit HexDigit;
fragment ExtendedUnicodeEscapeSequence: 'u' '{' HexDigit+ '}';
fragment SingleEscapeCharacter:         ['"\\bfnrtv];

fragment NonEscapeCharacter: ~['"\\bfnrtv0-9xu\r\n];
fragment EscapeCharacter:    SingleEscapeCharacter | [0-9] | [xu];
fragment LineContinuation:   '\\' [\r\n\u2028\u2029];

fragment DoubleStringCharacter
    : ~["\\\r\n]
    | '\\' EscapeSequence
    | LineContinuation;
fragment SingleStringCharacter
    : ~['\\\r\n]
    | '\\' EscapeSequence
    | LineContinuation;

eos: SemiColon | LineTerminator | EOF;

// Render
makeupStatement: element;

element
    : '<' Identifier attribute* '>' content '<' '/' Identifier '>'
    | '<' Identifier attribute* '/>';

content: (element)*;

attribute: Identifier '=' (StringLiteral | block);
