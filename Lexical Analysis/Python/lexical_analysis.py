import ply.lex as lex
import sys
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow,QApplication

reserved = {
    'if'        :   'IF',
    'then'      :   'THEN',
    'else'      :   'ELSE',
    'while'     :   'WHILE',
    'do'        :   'DO',
    'for'       :   'FOR',
    'switch'    :   'SWITCH',
    'case'      :   'CASE',
    'default'   :   'DEFAULT',
    'break'     :   'BREAK',
    'continue'  :   'CONTINUE',
    'int'       :   'INT',
    'float'     :   'FLOAT',
    'double'    :   'DOUBLE',
    'void'      :   'VOID',
    'struct'    :   'STRUCT',
    'const'     :   'CONST',
    'unsigned'  :   'UNSIGNED',
    'return'    :   'RETURN',
}

tokens=[
    'ID',
    'FNUM',
    'INUM',
    'PLUS',
    'MINUS',
    'TIMES',
    'DIVIDE',
    'LPAREN',
    'RPAREN',
    'MOD',
    'ASSIGN',
    'GREATER',
    'LESS',
    'POINT',
    'AND',
    'OR',
    'NOT',
    'LNOT',
    'XOR',
    'SEMI',
    'LBRACK',
    'RBRACK',
    'LBRACE',
    'RBRACE',
    'POUND',
    'EQUAL',
    'LAND',
    'LOR',
    'UNEQU',
    'SHL',
    'SHR',
]+list(reserved.values())

t_LAND      = r'&&'
t_LOR       = r'\|\|'
t_UNEQU     = r'!='
t_SHL       = r'<<'
t_SHR       = r'>>'
t_EQUAL     = r'=='
t_PLUS      = r'\+'
t_MINUS     = r'\-'
t_TIMES     = r'\*'
t_DIVIDE    = r'/'
t_LPAREN    = r'\('
t_RPAREN    = r'\)'
t_MOD       = r'%'
t_ASSIGN    = r'='
t_GREATER   = r'>'
t_LESS      = r'<'
t_POINT     = r'\.'
t_AND       = r'&'
t_OR        = r'\|'
t_NOT       = r'~'
t_LNOT      = r'!'
t_XOR       = r'\^'
t_SEMI      = r';'
t_LBRACK    = r'\['
t_RBRACK    = r'\]'
t_LBRACE    = r'\{'
t_RBRACE    = r'\}'
t_POUND     = '\#.*'

def t_FNUM(t):
    r'-?([1-9]\d*.\d*|0.\d*[1-9]\d*)'
    t.value = (t.value)
    return t

def t_INUM(t):
    r'-?\d+'
    t.value = int(t.value)
    return t

def t_ID(t):
    r'[a-zA-Z_][a-zA-Z0-9]*'
    t.type = reserved.get(t.value, 'ID')
    return t

def t_COMMENT(t):
    r'//.*'
    pass

def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

t_ignore  = ' \t'

def t_error(t):
    print ("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)


qtCreatorFile = r"lexical_analysis.ui" #uifile name

Ui_MainWindow,QtBaseClass = uic.loadUiType(qtCreatorFile)

class MyApp(QMainWindow,Ui_MainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.begin.clicked.connect(self.showresualt)
        self.data = open('test.txt').read()
        self.input.setText(self.data)

    def showresualt(self):
        lexer.input(self.data)
        file_obj = open('result.txt', 'w')
        file_obj.write('TYPE' + '\t| ' + 'VALUE' + '\n')

        file_obj.write('--------' + '+' + '--------' + '\n')
        while True:
            tok = lexer.token()
            if not tok: break
            file_obj.write(str(tok.type) + '\t| ' + str(tok.value) + '\n')
            self.output.append(str(tok.type) + '\t'*2 + str(tok.value) )
        file_obj.close()

if __name__ == "__main__":
    lexer = lex.lex()
    app = QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())








